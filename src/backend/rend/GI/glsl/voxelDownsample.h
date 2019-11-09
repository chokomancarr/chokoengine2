#pragma once
namespace glsl {
	const char voxelDownsampleVert[] = R"(
#version 330 core

flat out int v2g_layer;

void main() {
	v2g_layer = (gl_VertexID / 6);
	int vid = gl_VertexID - v2g_layer * 6;
	float y = -1;
	if (vid > 1 && vid < 5) y = 1;
	gl_Position = vec4(mod(vid, 2)*2-1, y, 0.5, 1.0);
}
)";

	const char voxelDownsampleGeom[] = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

flat in int[] v2g_layer;

flat out int g2f_layer;

void main() {
	gl_Position = gl_in[0].gl_Position;
	gl_Layer = v2g_layer[0];
	g2f_layer = gl_Layer;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	gl_Layer = v2g_layer[0];
	g2f_layer = gl_Layer;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	gl_Layer = v2g_layer[0];
	g2f_layer = gl_Layer;
	EmitVertex();
	EndPrimitive();
}
)";

	const char voxelDownsampleFrag[] = R"(
#version 330 core

flat in int g2f_layer;

uniform int sz;
uniform sampler3D tex;
uniform float mip;

out vec4 outColor;

void main() {
	float isz = 1.0 / sz;

	vec3 coord = vec3(
		gl_FragCoord.x,
		gl_FragCoord.y,
		g2f_layer) * isz;

	outColor = vec4(intBitsToFloat(5000), intBitsToFloat(10000), intBitsToFloat(5000), 1);//textureLod(tex, coord, mip);
}
)";
}