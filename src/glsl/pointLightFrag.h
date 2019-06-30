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
uniform int falloff;

out vec4 fragCol;

float length2(vec3 v) {
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

float fresnel(vec3 fwd, vec3 nrm) {
	return pow(1-dot(-fwd, nrm), 5);
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

	vec4 dc = vec4(uv.x*2-1, uv.y*2-1, z*2-1, 1);
	vec4 wPos = _IP*dc;
	wPos /= wPos.w;
	vec4 wPos2 = _IP*vec4(uv.x*2-1, uv.y*2-1, -1, 1);
	wPos2 /= wPos2.w;
	vec3 fwd = normalize(wPos.xyz - wPos2.xyz);
	
	fragCol.rgba = vec4(0, 0, 0, 0);
	if (z < 1) {
		vec3 p2l = lightPos - wPos.xyz;
		vec3 p2li = normalize(p2l);
		fragCol.rgb = vec3(1, 1, 1) * max(dot(p2li, normal), 0);
	}
	return;
}
)";
}