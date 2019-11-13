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
uniform int voxelSize;

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
	return decode(textureLod(emitTexX, loc.xyz, lod).xyz, -nrm.x)
		+ decode(textureLod(emitTexY, loc.xyz, lod).xyz, -nrm.y)
		+ decode(textureLod(emitTexZ, loc.xyz, lod).xyz, -nrm.z);
}

vec3 sampleLightAt(vec3 loc, vec3 nrm, int lod) {
	//return lightAt(loc, nrm, lod);
	float sz = float(voxelSize >> lod);
	float isz = 0.5 / sz;
	vec3 l0 = loc - isz;
	vec3 l1 = loc + isz;
	vec3 lp = (loc - l0) * sz;

	vec3 nn = mix( lightAt(l0, nrm, lod),
		lightAt(vec3(l0.x, l0.y, l1.z), nrm, lod), lp.z);
	vec3 pn = mix( lightAt(vec3(l1.x, l0.y, l0.z), nrm, lod),
		lightAt(vec3(l1.x, l0.y, l1.z), nrm, lod), lp.z);
	vec3 np = mix( lightAt(vec3(l0.x, l1.y, l0.z), nrm, lod),
		lightAt(vec3(l0.x, l1.y, l1.z), nrm, lod), lp.z);
	vec3 pp = mix( lightAt(vec3(l1.x, l1.y, l0.z), nrm, lod),
		lightAt(vec3(l1.x, l1.y, l1.z), nrm, lod), lp.z);

	return mix(
		mix(nn, np, lp.y),
		mix(pn, pp, lp.y),
		lp.x);
}

vec3 sampleLightAtLod(vec3 loc, vec3 nrm, float lod) {
	return mix(
		lightAt(loc, nrm, int(floor(lod))),
		lightAt(loc, nrm, int(ceil(lod))),
		lod - floor(lod));
}

const float r3 = sqrt(3) * 0.5;

const vec3 diffDirs[5] = vec3[](
	vec3(1, 0, 0), vec3(0.5, r3, 0), vec3(0.5, -r3, 0),
	vec3(0.5, 0, r3), vec3(0.5, 0, -r3)
);

void main () {
	vec2 uv = gl_FragCoord.xy / screenSize;
	vec4 diffuse = texture(inGBuf0, uv);
	vec3 normal = texture(inGBuf1, uv).xyz;
	vec4 gbuf2 = texture(inGBuf2, uv);
	float metallic = gbuf2.x;
	float rough = gbuf2.y;
	float occlu = gbuf2.z;
	float z = texture(inGBufD, uv).x;

	vec3 tan = normalize(cross(normal, vec3((abs(normal.y) > 0.99) ? 1 : 0, 1, 0)));
	vec3 bitan = normalize(cross(normal, tan));

	vec4 wPos = _IP * vec4(uv.x*2-1, uv.y*2-1, z*2-1, 1);
	wPos /= wPos.w;
	vec4 wPos2 = _IP * vec4(uv.x*2-1, uv.y*2-1, -1, 1);
	wPos2 /= wPos2.w;
	vec3 fwd = normalize(wPos.xyz - wPos2.xyz);
	
	fragCol = vec4(0, 0, 0, 0);
	if (z < 1) {
		float dpos = voxelUnit;

		vec3 fcol = vec3(0, 0, 0);
		for (int n = 0; n < 1; n++) {
			vec3 dd = diffDirs[n];
			vec3 nrm = normal * dd.x + tan * dd.y + bitan * dd.z;
			vec3 wp = wPos.xyz;

			for (int a = 0; a <= voxelMips; a++) {
				vec4 loc2 = voxelMat * vec4(wp, 1);
				loc2.xyz = (loc2.xyz / loc2.w) * 0.5 + 0.5;
				fcol += sampleLightAtLod(loc2.xyz, nrm, a);
				wp += nrm * dpos;
				dpos *= 2;
			}
		}
		fragCol.rgb = diffuse.rgb * fcol * 0.2;
	}
}
)";
}