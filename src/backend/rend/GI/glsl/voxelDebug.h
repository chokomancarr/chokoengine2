#pragma once
namespace glsl {
	const char voxelDebugVert[] = R"(
#version 330 core

uniform int num;
uniform mat4 _P;

out vec3 v2g_uvw;

void main() {
	int x = gl_VertexID / num / num;
	int ty = gl_VertexID - x * num * num;
	int y = ty / num;
	int z = int(mod(ty, num));
	v2g_uvw = vec3(x, y, z) / (num - 1.0);
	gl_Position = _P * vec4(v2g_uvw * 2 - 1, 1);
}
)";

	const char voxelDebugGeom[] = R"(
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vec3 v2g_uvw[];

uniform float ptsz;

out vec3 g2f_uvw;

void main() {
	vec4 pos = gl_in[0].gl_Position;
	pos /= pos.w;

	gl_Position = pos - vec4(-ptsz, -ptsz, 0, 0);
	g2f_uvw = v2g_uvw[0];
	EmitVertex();
	gl_Position = pos - vec4(-ptsz, ptsz, 0, 0);
	g2f_uvw = v2g_uvw[0];
	EmitVertex();
	gl_Position = pos - vec4(ptsz, -ptsz, 0, 0);
	g2f_uvw = v2g_uvw[0];
	EmitVertex();
	gl_Position = pos - vec4(-ptsz, ptsz, 0, 0);
	g2f_uvw = v2g_uvw[0];
	EmitVertex();
	gl_Position = pos - vec4(ptsz, ptsz, 0, 0);
	g2f_uvw = v2g_uvw[0];
	EmitVertex();
	gl_Position = pos - vec4(ptsz, -ptsz, 0, 0);
	g2f_uvw = v2g_uvw[0];
	EmitVertex();
	EndPrimitive();
}
)";

	const char voxelDebugFrag[] = R"(
#version 330 core

in vec3 g2f_uvw;

uniform sampler3D tex;

out vec4 fragCol;

void main() {
	fragCol = texture(tex, g2f_uvw);
}
)";
}