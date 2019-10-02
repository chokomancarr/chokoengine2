#pragma once
namespace glsl {
	const char pointLightFrag_Soft[] = R"(
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

out vec4 fragCol;

uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float rand( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float rand( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float rand( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }

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

float vec2depth(vec3 v)
{
    vec3 va = abs(v);
    float mv = max(va.x, max(va.y, va.z));

    float f = lightDst;
    float n = lightRad;
    float nv = (f+n) / (f-n) - (2*f*n)/(f-n)/mv;
    return linearz((nv + 1.0) * 0.5);
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
		//float lpn = max(closest_point(wPos.xyz, refl, lightPos), 0);
		vec3 pn = wPos.xyz;// + refl * lpn;
		vec3 dpn = pn - lightPos;
		dpn *= min(lightRad / length(dpn), 1);
		pn = lightPos + dpn;
		vec3 p2l = pn - wPos.xyz;
		if (length2(p2l) < lightRad * lightRad) {
			fragCol.rgb = lightCol * lightStr * occlu;
			return;
		}
		vec3 p2li = normalize(p2l);
		vec3 diffCol = diffuse.rgb * max(dot(p2li, normal), 0);
		vec3 hv = normalize(p2li - fwd);
		float reflStr = ggx(normal, hv, rough);
		vec3 reflCol = mix(vec3(1, 1, 1), diffuse.rgb, metallic) * reflStr;

		float not_shadow = 1;
		
		if (shadowStr > 0) {
			float rr2 = 1;
			float pz = vec2depth(p2l);
			float sz0 = shadowAt(-p2l);
			if (sz0 < pz - shadowBias) {
				rr2 = min((pz - sz0) * lightDst / lightRad / sz0, 1);
				not_shadow = 0;
			}
			float rnd = rand(wPos.xyz);
			vec3 lt1 = vec3(1, 0, 0);
			if (abs(p2li.x) > 0.99) lt1 = vec3(0, 1, 0);
			vec3 lt2 = normalize(cross(p2li, lt1));
			lt1 = normalize(cross(p2li, lt2));
			for (int a = 1; a < shadowSmps; a++) {
				float rt = rand(rnd) * 2 * 3.14159;
				float rr = sqrt(rand(rt)) * lightRad;
				rnd = rr;
				float sz = shadowAt(-p2l + (lt1 * sin(rt) + lt2 * cos(rt)) * rr * rr2);
				not_shadow += (sz >= pz - shadowBias) ? 1 : (1 - shadowStr);
			}
			not_shadow /= shadowSmps;
		}
		
		fragCol.rgb = mix(diffCol, reflCol, mix(fres, 1, metallic)) * lightCol * lightStr * occlu * get_falloff(length(p2l)) * not_shadow;
	}
	return;
}
)";
}