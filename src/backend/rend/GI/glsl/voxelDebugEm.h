#pragma once
namespace glsl {
	const char voxelDebugEmVert[] = R"(
#version 420 core

uniform int num;

uniform mat4 _VP;

uniform sampler3D emitTexX;
uniform sampler3D emitTexY;
uniform sampler3D emitTexZ;
uniform float mip;

out vec3 normal;
out vec3 emit;
out vec3 cpos;

const int cubeids[36] = int[](
	3, 0, 6,	0, 7, 6,
	1, 2, 4,	2, 5, 4,
	3, 2, 0,	2, 1, 0,
	7, 4, 6,	4, 5, 6,
	2, 3, 5,	3, 6, 5,
	0, 1, 7,	1, 4, 7
);

const float cubeverts[24] = float[](
	-1, -1, -1,	 1, -1, -1,
	 1, -1,  1,	-1, -1,  1,
	 1,  1, -1,	 1,  1,  1,
	-1,  1,  1,	-1,  1, -1
);

const vec3 normals[6] = vec3[](
	vec3(0, 0, -1), vec3(1, 0, 0),
	vec3(0, 0, 1), vec3(-1, 0, 0),
	vec3(0, -1, 0), vec3(0, 1, 0)
);

const vec3 normals2[6] = vec3[](
	vec3(1, 0, 1), vec3(1, 0, 0),
	vec3(0, 0, 1), vec3(1, 1, 0),
	vec3(0, 1, 1), vec3(0, 1, 0)
);

vec3 decodeE(vec3 v, int i) {
	return vec3(
		unpackHalf2x16(floatBitsToUint(v.x))[i],
		unpackHalf2x16(floatBitsToUint(v.y))[i],
		unpackHalf2x16(floatBitsToUint(v.z))[i]
	);
}

void main() {
	int vid = gl_VertexID / 36;
	int a = gl_VertexID - vid * 36;
	int x = vid / num / num;
	int ty = vid - x * num * num;
	int y = ty / num;
	int z = int(mod(ty, num));

	vec3 pos = vec3(x, y, z) + 0.5;
	vec3 uvw = pos / num;
	vec3 texvals[3] = vec3[](
		textureLod(emitTexX, uvw, mip).xyz,
		textureLod(emitTexY, uvw, mip).xyz,
		textureLod(emitTexZ, uvw, mip).xyz
	);

	int fi = a / 6; //which face
	normal = normals2[fi];
	int fi2 = fi / 2; //which axis

	float scl = 0;

	vec3 emits[6];
	for (int a = 0; a < 6; a++) {
		emits[a] = decodeE(texvals[a / 2], a - ((a / 2) * 2));
		scl = max(scl, ceil(min(emits[a].x + emits[a].y + emits[a].z, 1)) * 0.5);
	}

	emit = emits[fi];

	int id = cubeids[a] * 3;
	cpos = vec3(cubeverts[id], cubeverts[id + 1], cubeverts[id + 2]);
	vec3 pos2 = pos + cpos * scl;
	vec4 posn = vec4(pos2 / num * 2 - 1, 1);
	gl_Position = _VP * posn;
}
)";

	const char voxelDebugEmFrag[] = R"(
#version 420 core

in vec3 normal;
in vec3 emit;
in vec3 cpos;

out vec4 fragCol;

void main() {
	float ed = abs(cpos.x) > 0.95 ? 1 : 0;
	ed += abs(cpos.y) > 0.95 ? 1 : 0;
	ed += abs(cpos.z) > 0.95 ? 1 : 0;

	if (ed > 1)
		fragCol = vec4(1, 0, 0, 1);
	else 
		fragCol = vec4(emit, 1);
}
)";
}