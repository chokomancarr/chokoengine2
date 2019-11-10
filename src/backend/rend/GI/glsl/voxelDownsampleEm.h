#pragma once
namespace glsl {
	const char voxelDownsampleEmVert[] = R"(
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

	const char voxelDownsampleEmGeom[] = R"(
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

	const char voxelDownsampleEmFrag[] = R"(
#version 420 core

layout (pixel_center_integer) in vec4 gl_FragCoord;

flat in int g2f_layer;

uniform int sz;
uniform sampler3D texX;
uniform sampler3D texY;
uniform sampler3D texZ;
uniform int mip;

layout (location=0) out vec4 outColorX;
layout (location=1) out vec4 outColorY;
layout (location=2) out vec4 outColorZ;

ivec3 offs[8] = ivec3[](
	ivec3(0, 0, 0), ivec3(1, 0, 0),
	ivec3(0, 1, 0), ivec3(1, 1, 0),
	ivec3(0, 0, 1), ivec3(1, 0, 1),
	ivec3(0, 1, 1), ivec3(1, 1, 1)
);

vec3 decode(vec3 v, int i) {
	return vec3(
		unpackHalf2x16(floatBitsToUint(v.x))[i],
		unpackHalf2x16(floatBitsToUint(v.y))[i],
		unpackHalf2x16(floatBitsToUint(v.z))[i]
	);
}

vec3 encode(vec3 e0, vec3 e1) {
	return vec3(
		uintBitsToFloat(packHalf2x16(vec2(e0.x, e1.x))),
		uintBitsToFloat(packHalf2x16(vec2(e0.y, e1.y))),
		uintBitsToFloat(packHalf2x16(vec2(e0.z, e1.z)))
	);
}

void main() {
	float isz = 1.0 / sz;

	ivec3 coord = ivec3(
		int(gl_FragCoord.x),
		int(gl_FragCoord.y),
		g2f_layer) * 2;

	vec3 x1 = vec3(0, 0, 0), y1 = vec3(0, 0, 0), z1 = vec3(0, 0, 0);
	vec3 x2 = vec3(0, 0, 0), y2 = vec3(0, 0, 0), z2 = vec3(0, 0, 0);

	for (int a = 0; a < 8; a++) {
		ivec3 crd = coord + offs[a];
		vec3 tv = texelFetch(texX, crd, mip).xyz;
		x1 += decode(tv, 0);
		x2 += decode(tv, 1);

		tv = texelFetch(texY, crd, mip).xyz;
		y1 += decode(tv, 0);
		y2 += decode(tv, 1);

		tv = texelFetch(texZ, crd, mip).xyz;
		z1 += decode(tv, 0);
		z2 += decode(tv, 1);
	}

	x1 *= 0.125;
	x2 *= 0.125;
	y1 *= 0.125;
	y2 *= 0.125;
	z1 *= 0.125;
	z2 *= 0.125;

	outColorX = vec4(encode(x1, x2), 0);
	outColorY = vec4(encode(y1, y2), 0);
	outColorZ = vec4(encode(z1, z2), 0);
}
)";
}