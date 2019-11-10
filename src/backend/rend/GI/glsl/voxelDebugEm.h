#pragma once
namespace glsl {
	const char voxelDebugEmVert[] = R"(
uniform int num;

uniform mat4 _VP;

uniform sampler3D emitTex;
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

void main() {
	int vid = gl_VertexID / 36;
	int a = gl_VertexID - vid * 36;
	int x = vid / num / num;
	int ty = vid - x * num * num;
	int y = ty / num;
	int z = int(mod(ty, num));

	vec3 pos = vec3(x, y, z);
	vec3 uvw = pos / (num - 1.0);
	vec4 texval = textureLod(emitTex, uvw, mip);

	float scl = ceil(texval.w) * 0.5;//ceil(min(texval.x + texval.y + texval.z, 1) * 0.5;

	int id = cubeids[a] * 3;
	cpos = vec3(cubeverts[id], cubeverts[id + 1], cubeverts[id + 2]);
	vec3 pos2 = pos + cpos * scl;
	vec4 posn = vec4(pos2 / (num - 1.0) * 2 - 1, 1);
	gl_Position = _VP * posn;

	int fi = a / 6;
	normal = normals2[fi];

	emit = texval.xyz;
}
)";

	const char voxelDebugEmFrag[] = R"(
in vec3 normal;
in vec3 emit;
in vec3 cpos;

out vec4 fragCol;

void main() {
	float mul = 1 - max(abs(cpos.x) + abs(cpos.y) + abs(cpos.z) - 2, 0);
	fragCol = vec4(emit * mul, 1);
}
)";
}