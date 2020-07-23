#pragma once
#include "inc/random.h"

namespace glsl {
	const std::string particleout_tf_v = R"(#version 330 core

layout(location=0) in vec4 inbuf0;
layout(location=1) in vec4 inbuf1;
layout(location=2) in vec4 inbuf2;

out vec3 v2g_pos;
out float v2g_scl;
out float v2g_rot;

void main () {
	float hp = inbuf0.x;
	float maxhp = inbuf0.y;
	float rot = inbuf0.z;
	vec3 pos = inbuf1.xyz;
	float scl = inbuf1.w;
	vec3 vel = inbuf2.xyz;
	float avel = inbuf2.w;

	v2g_pos = pos;
	v2g_scl = scl;
	v2g_rot = rot;
}
)";

	const std::string particleout_tf_g = R"(#version 330 core

#define PI 3.14159

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in vec3[] v2g_pos;
in float[] v2g_scl;
in float[] v2g_rot;

uniform vec3 camUp;
uniform vec3 camRight;
uniform vec3 camFwd;

out vec3 out_pos;
out vec3 out_normal;
out vec3 out_tangent;
out vec2 out_texCoord;

void main () {
	float th = v2g_rot[0] * PI / 180;
	float cth = cos(th);
	float sth = sin(th);
	vec3 dx = cth * camRight - sth * camUp;
	vec3 dy = cth * camUp + sth * camRight;

	float lut[12] = float[](
		0, 0,	1, 0,	0, 1,
		1, 0,	1, 1,	0, 1
	);
	
	for (int a = 0; a < 6; a++) {
		float px = lut[a*2];
		float py = lut[a*2+1];
		
		out_pos = v2g_pos[0] + (px*2-1)*dx + (py*2-1)*dy;
		out_normal = -camFwd;
		out_tangent = camRight;
		out_texCoord = vec2(px, py);
		EmitVertex();
		if (a == 2) {
			EndPrimitive();
		}
	}
	EndPrimitive();
}
)";
}