#pragma once
namespace glsl {
	const char voxelDebugVert[] = R"(
uniform int num;

uniform mat4 _VP;

uniform sampler3D tex;

out vec3 normal;
out float occlu;
out vec3 cpos;

const int cubeids[36] = int[](
	0, 1, 7,	1, 4, 7,
	1, 2, 4,	2, 5, 4,
	2, 3, 5,	3, 6, 5,
	3, 0, 6,	0, 7, 6,
	3, 2, 0,	2, 1, 0,
	7, 4, 6,	4, 5, 6
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
	float texval = texture(tex, uvw).r;
	float scl = ceil(texval) * 0.5;

	int id = cubeids[a] * 3;
	cpos = vec3(cubeverts[id], cubeverts[id + 1], cubeverts[id + 2]);
	vec3 pos2 = pos + cpos * scl;
	vec4 posn = vec4(pos2 / (num - 1.0) * 2 - 1, 1);
	gl_Position = _VP * posn;
	normal = normals2[a / 6];
	occlu = texval;
}
)";

	const char voxelDebugFrag[] = R"(
in vec3 normal;
in float occlu;
in vec3 cpos;

out vec4 fragCol;

void main() {
	float mul = 1 - max(abs(cpos.x) + abs(cpos.y) + abs(cpos.z) - 2, 0);
	fragCol = vec4(normal * mul, 1);//vec4(occlu, occlu, occlu, 1);
}
)";
}