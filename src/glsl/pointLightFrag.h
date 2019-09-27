#pragma once
namespace glsl {
	const char pointLightFrag[] = R"(
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

out vec4 fragCol;

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

void main () {
	vec2 uv = gl_FragCoord.xy / screenSize;
	vec4 diffuse = texture(inGBuf0, uv);
	vec3 normal = texture(inGBuf1, uv).xyz;

	vec4 gbuf2 = texture(inGBuf2, uv);
	float metallic = gbuf2.x;
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
		float lpn = max(closest_point(wPos.xyz, refl, lightPos), 0);
		vec3 pn = wPos.xyz + refl * lpn;
		vec3 dpn = pn - lightPos;
		dpn *= min(lightRad / length(dpn), 1);
		pn = lightPos + dpn;
		vec3 p2l = pn - wPos.xyz;
		vec3 p2li = normalize(p2l);
		vec3 diffCol = diffuse.rgb * max(dot(p2li, normal), 0);
		vec3 hv = normalize(p2li - fwd);
		float reflStr = ggx(normal, hv, rough);
		vec3 reflCol = mix(vec3(1, 1, 1), diffuse.rgb, metallic) * reflStr;
		fragCol.rgb = mix(diffCol, reflCol, mix(fres, 1, metallic)) * lightCol * lightStr * occlu * get_falloff(length(p2l));
	}
	return;
}
)";
}