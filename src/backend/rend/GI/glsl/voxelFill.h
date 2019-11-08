#pragma once
namespace glsl {
	const char voxelFillVert[] = R"(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;

uniform mat4 _M;
uniform mat4 _MVP;

out vec3 v2g_uvw;
out vec3 v2g_nrm;

void main() {
	vec4 p = _MVP * vec4(pos, 1);
	v2g_uvw = p.xyz / p.w;
	v2g_nrm = (_M * vec4(nrm, 0)).xyz;
}
)";

	const char voxelFillGeom[] = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3[] v2g_uvw;
in vec3[] v2g_nrm;

uniform int layerCount;

out vec3 g2f_uvw;
out vec3 g2f_nrm;

void main() {
	vec3 p1 = v2g_uvw[0];
	vec3 p2 = v2g_uvw[1];
	vec3 p3 = v2g_uvw[2];
	
	float dx = max(
		abs(p2.x - p1.x),
		abs(p3.x - p1.x)
	);
	float dy = max(
		abs(p2.y - p1.y),
		abs(p3.y - p1.y)
	);
	float dz = max(
		abs(p2.z - p1.z),
		abs(p3.z - p1.z)
	);

	if (dz < dy && dz < dx) {
		//z axis is smallest, project to z
		//already on z, do nothing
	}
	else if (dy < dx && dy < dz) {
		//y axis is smallest, project to y
		p1 = p1.zxy;
		p2 = p2.zxy;
		p3 = p3.zxy;
	}
	else {
		//x axis is smallest, project to x
		p1 = p1.yzx;
		p2 = p2.yzx;
		p3 = p3.yzx;
	}

	gl_Position = vec4(p1, 1);
	g2f_uvw = (v2g_uvw[0] * 0.5 + 0.5) * layerCount;
	g2f_nrm = v2g_nrm[0];
	EmitVertex();
	gl_Position = vec4(p2, 1);
	g2f_uvw = (v2g_uvw[1] * 0.5 + 0.5) * layerCount;
	g2f_nrm = v2g_nrm[0];
	EmitVertex();
	gl_Position = vec4(p3, 1);
	g2f_uvw = (v2g_uvw[2] * 0.5 + 0.5) * layerCount;
	g2f_nrm = v2g_nrm[0];
	EmitVertex();
	EndPrimitive();
}
)";

	const char voxelFillFrag[] = R"(
#version 420 core

in vec3 g2f_uvw;
in vec3 g2f_nrm;

uniform layout(binding=3, rgba32f) writeonly image3D voxelTex;

void main() {
	ivec3 coord;
	coord.x = int(g2f_uvw.x);
	coord.y = int(g2f_uvw.y);
	coord.z = int(g2f_uvw.z);

	vec3 nrm = normalize(g2f_nrm);
	uint x = (uint(max(-nrm.x, 0) * 10000) << 16) + uint(max(nrm.x, 0) * 10000);
	uint y = (uint(max(-nrm.y, 0) * 10000) << 16) + uint(max(nrm.y, 0) * 10000);
	uint z = (uint(max(-nrm.z, 0) * 10000) << 16) + uint(max(nrm.z, 0) * 10000);

	imageStore(voxelTex, coord, vec4(//g2f_nrm, 1));
		uintBitsToFloat(x), uintBitsToFloat(y), uintBitsToFloat(z), 1));
}
)";
}