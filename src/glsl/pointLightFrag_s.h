#pragma once
#include "inc/random.h"

namespace glsl {
	const std::string pointLightFrag_Soft = R"(
uniform mat4 _IP;
uniform vec2 screenSize;

uniform sampler2D inGBuf0;
uniform sampler2D inGBuf1;
uniform sampler2D inGBuf2;
uniform sampler2D inGBufD;

uniform vec3 lightPos;
uniform float lightStr;
uniform float lightRad;
uniform float lightDst;
uniform vec3 lightCol;
uniform int falloff;
uniform samplerCube shadowMap;
uniform float shadowStr;
uniform float shadowBias;
uniform int shadowSmps;
uniform float transparent;

out vec4 fragCol;

)" + inc::rand + R"(

float length2(vec3 v) {
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

float fresnel(vec3 fwd, vec3 nrm) {
	return pow(1-dot(-fwd, nrm), 5);
}

float closest_point(vec3 pos, vec3 dir, vec3 point) {
	return dot(point - pos, dir);
}

float ggx(vec3 n, vec3 h, float r) {
    float NoH = dot(n,h);
    float r2 = r * r;
    float NoH2 = NoH * NoH;
    float den = NoH2 * r2 + (1 - NoH2);
	if (NoH <= 0) return 0;
    return r2 / ( 3.14159 * den * den );
}

float get_falloff(float d) {
	float fall = max(1 - length(d) / lightDst, 0);
	if (falloff == 0) return ceil(fall);
	if (falloff == 1) return fall;
	return fall * fall;
}

float linearz(float z) {
	return (2 * lightRad) / (lightDst + lightRad - z * (lightDst - lightRad));
}

float shadowAt(vec3 v) {
	return linearz(texture(shadowMap, normalize(v)).r);
}

float vec2depth(vec3 v) {
    vec3 va = abs(v);
    float mv = max(va.x, max(va.y, va.z));

    float f = lightDst;
    float n = lightRad;
    float nv = (f+n) / (f-n) - (2*f*n)/(f-n)/mv;
    return linearz((nv + 1.0) * 0.5);
}

float cocrad(float shadowZ, float myZ) {
	return (lightDst - lightRad) * (myZ - shadowZ) / shadowZ * lightRad;
}

void main () {
	vec2 uv = gl_FragCoord.xy / screenSize;
	vec4 diffuse = texture(inGBuf0, uv);
	vec3 normal = texture(inGBuf1, uv).xyz;

	vec4 gbuf2 = texture(inGBuf2, uv);
	float metallic = gbuf2.x * (1 - transparent);
	float rough = gbuf2.y;
	float occlu = gbuf2.z;
	float z = texture(inGBufD, uv).x;

	vec4 wPos = _IP * vec4(uv.x*2-1, uv.y*2-1, z*2-1, 1);
	wPos /= wPos.w;
	vec4 wPos2 = _IP * vec4(uv.x*2-1, uv.y*2-1, -1, 1);
	wPos2 /= wPos2.w;
	vec3 fwd = normalize(wPos.xyz - wPos2.xyz);
	
	fragCol.rgba = vec4(0, 0, 0, 0);
	if (z < 1) {
		vec3 refl = normalize(reflect(fwd, normal));
		float fres = mix(fresnel(fwd, normal), 1, 0.1);
		//float lpn = max(closest_point(wPos.xyz, refl, lightPos), 0);
		vec3 pn = wPos.xyz;// + refl * lpn;
		vec3 dpn = pn - lightPos;
		dpn *= min(lightRad / length(dpn), 1);
		pn = lightPos + dpn;
		vec3 p2l = pn - wPos.xyz;
		float p2ll2 = length2(p2l);
		float p2ll = sqrt(p2ll2);
		if (length2(p2l) < lightRad * lightRad) {
			fragCol.rgb = lightCol * lightStr * occlu;
			return;
		}
		vec3 p2li = normalize(p2l);
		vec4 diffCol = vec4(diffuse.rgb * max(dot(p2li, normal), 0), diffuse.a);
		vec3 hv = normalize(p2li - fwd);
		float reflStr = ggx(normal, hv, rough);
		vec4 reflCol = vec4(mix(vec3(1, 1, 1), diffuse.rgb, metallic), reflStr);
		reflCol = mix(vec4(reflCol.rgb * reflCol.w, 0), reflCol, transparent);

		float not_shadow = 1;
		int not_shadown = 1;
		
		if (shadowStr > 0 && transparent == 0) {
			float rr2 = 0;
			float pz = vec2depth(p2l);
			float sz0 = shadowAt(-p2l);
			if (sz0 < pz - shadowBias) {
				rr2 = cocrad(sz0, pz);
				not_shadow = 1 - shadowStr;
			}
			float rnd = rand(wPos.xyz);
			vec3 lt1 = vec3(1, 0, 0);
			if (abs(p2li.x) > 0.99) lt1 = vec3(0, 1, 0);
			vec3 lt2 = normalize(cross(p2li, lt1));
			lt1 = normalize(cross(p2li, lt2));
			for (int a = 1; a < shadowSmps; a++) {
				float rt = rand(rnd) * 2 * 3.14159;
				float rr = sqrt(rand(rt));
				rnd = rr;
				vec3 sp = -p2l + (lt1 * sin(rt) + lt2 * cos(rt)) * rr * rr2;
				float sz = shadowAt(sp);
				float szl = mix(lightRad, lightDst, sz);
				float rr2s = cocrad(sz, pz); //effect radius for this sample
				sp = normalize(sp) * p2ll;
				float rrs = length2(p2l - sp); //distance from sample
				//if (rrs < (rr2s * rr2s)) {
					not_shadow += (sz >= (pz - shadowBias)) ? 1 : (1 - shadowStr);
					not_shadown += 1;
				//}
			}
			not_shadow /= not_shadown;
		}
		
		fragCol = mix(diffCol, reflCol, mix(fres, 1, metallic)) * vec4(lightCol * lightStr * occlu * get_falloff(length(p2l)) * not_shadow, 1);
	}
}
)";
}