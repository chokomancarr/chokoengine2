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

layout (pixel_center_integer) in vec4 gl_FragCoord;

flat in int g2f_layer;

uniform int sz;
uniform sampler3D tex;
uniform float mip;

out vec4 outColor;

vec3 offs[8] = vec3[](
	vec3(0, 0, 0), vec3(1, 0, 0),
	vec3(0, 1, 0), vec3(1, 1, 0),
	vec3(0, 0, 1), vec3(1, 0, 1),
	vec3(0, 1, 1), vec3(1, 1, 1)
);

vec2 decode(float f) {
	uint u = floatBitsToUint(f);
	vec2 res;
	uint i = uint(u >> 16);
	res.x = float(i) * 0.0001;
	i = u & 0xffffu;
	res.y = float(i) * 0.0001;
	return res;
}

float encode(vec2 v) {
	uint u1 = uint(v.x * 10000);
	uint u2 = uint(v.y * 10000);
	uint ru = uint(u1 << 16u) + u2;
	return uintBitsToFloat(ru);
}

void main() {
	float isz = 1.0 / sz;

	vec3 coord = vec3(
		gl_FragCoord.x,
		gl_FragCoord.y,
		g2f_layer) * isz;

	vec2 x = vec2(0, 0), y = vec2(0, 0), z = vec2(0, 0);
	float w = 0;
	
	for (int a = 0; a < 8; a++) {
		vec4 val = textureLod(tex, coord + offs[a] * isz * 0.5, mip);
		x += decode(val.x);
		y += decode(val.y);
		z += decode(val.z);
		w += val.w;
	}

	x *= 0.125;
	y *= 0.125;
	z *= 0.125;
	w *= 0.125;
	
	outColor = vec4(encode(x), encode(y), encode(z), w);
}
)";
}