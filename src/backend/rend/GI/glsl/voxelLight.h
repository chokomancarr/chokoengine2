#pragma once
namespace glsl {
	const char voxelLightFrag[] = R"(
#version 420 core

uniform mat4 _IP;
uniform vec2 screenSize;

uniform sampler2D inGBuf0;
uniform sampler2D inGBuf1;
uniform sampler2D inGBuf2;
uniform sampler2D inGBufD;

uniform int voxelMips;
uniform mat4 voxelMat;
uniform float voxelUnit;

uniform sampler3D emitTexX;
uniform sampler3D emitTexY;
uniform sampler3D emitTexZ;

out vec4 fragCol;

vec3 decode(vec3 v, float n) {
	int i = int(round(n * 0.5 + 0.5));
	return vec3(
		unpackHalf2x16(floatBitsToUint(v.x))[i],
		unpackHalf2x16(floatBitsToUint(v.y))[i],
		unpackHalf2x16(floatBitsToUint(v.z))[i]
	) * abs(n);
}

vec3 lightAt(vec3 loc, vec3 nrm, float lod) {
	vec4 loc2 = voxelMat * vec4(loc, 1);
	loc2.xyz = (loc2.xyz / loc2.w) * 0.5 + 0.5;
	return decode(textureLod(emitTexX, loc2.xyz, lod).xyz, -nrm.x)
		+ decode(textureLod(emitTexY, loc2.xyz, lod).xyz, -nrm.y)
		+ decode(textureLod(emitTexZ, loc2.xyz, lod).xyz, -nrm.z);
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
	
	fragCol = vec4(0, 0, 0, 0);
	if (z < 1) {
		float dpos = voxelUnit;
		for (int a = 0; a <= voxelMips; a++) {
			fragCol.rgb += lightAt(wPos.xyz, normal, a);
			wPos.xyz += normal * dpos;
			dpos *= 2;
		}
		fragCol.rgb *= diffuse.rgb;
	}
}
)";
}