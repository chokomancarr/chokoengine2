#pragma once
namespace glsl {
	const char voxelFillLightPointVert[] = R"(
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

	const char voxelFillLightPointGeom[] = R"(
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

	const char voxelFillLightPointFrag[] = R"(
#version 330 core

layout (pixel_center_integer) in vec4 gl_FragCoord;

flat in int g2f_layer;

uniform mat4 _V2W;

uniform int sz;
uniform sampler3D diffTexX;
uniform sampler3D diffTexY;
uniform sampler3D diffTexZ;
uniform sampler3D emitTexX;
uniform sampler3D emitTexY;
uniform sampler3D emitTexZ;

uniform vec3 lightPos;
uniform float lightStr;
uniform float lightRad;
uniform float lightDst;
uniform vec3 lightCol;
uniform int falloff;
uniform samplerCube shadowMap;
uniform float shadowStr;
uniform float shadowBias;

layout (location=0) out vec4 outColorX;
layout (location=1) out vec4 outColorY;
layout (location=2) out vec4 outColorZ;

float get_falloff(float d) {
	float fall = max(1 - length(d) / lightDst, 0);
	if (falloff == 0) return ceil(fall);
	if (falloff == 1) return fall;
	return fall * fall;
}

float vec2depth(vec3 v)
{
    vec3 va = abs(v);
    float mv = max(va.x, max(va.y, va.z));

    float f = lightDst;
    float n = lightRad;
    float nv = (f+n) / (f-n) - (2*f*n)/(f-n)/mv;
    return (nv + 1.0) * 0.5;
}

vec3 decodeD(vec3 v, int i) {
	i = 1 - i;
	return vec3(
		float((floatBitsToUint(v.x) >> (16 * i)) & 0xffffu) * 0.0001,
		float((floatBitsToUint(v.y) >> (16 * i)) & 0xffffu) * 0.0001,
		float((floatBitsToUint(v.z) >> (16 * i)) & 0xffffu) * 0.0001
	);
}

vec3 decodeE(vec3 v, int i) {
	return vec3(
		unpackHalf2x16(floatBitsToUint(v.x))[i],
		unpackHalf2x16(floatBitsToUint(v.y))[i],
		unpackHalf2x16(floatBitsToUint(v.z))[i]
	);
}

vec3 encodeE(vec3 e0, vec3 e1) {
	return vec3(
		uintBitsToFloat(packHalf2x16(vec2(e0.x, e1.x))),
		uintBitsToFloat(packHalf2x16(vec2(e0.y, e1.y))),
		uintBitsToFloat(packHalf2x16(vec2(e0.z, e1.z)))
	);
}

const vec3 normals[6] = vec3[](
	vec3(-1, 0, 0), vec3(1, 0, 0),
	vec3(0, -1, 0), vec3(0, 1, 0),
	vec3(0, 0, -1), vec3(0, 0, 1)
);

void main() {
	vec3 coord = vec3(
		gl_FragCoord.x,
		gl_FragCoord.y,
		g2f_layer);

	vec3 pos = vec3(x, y, z) + 0.5;
	vec4 wpos = _V2W * vec4(pos / sz * 2 - 1, 1);
	wpos /= wpos.w;

	//get diffuse colors

	vec3 diffVs[3] = vec3[](
		texelFetch(diffTexX, coord),
		texelFetch(diffTexY, coord),
		texelFetch(diffTexZ, coord)
	);

	vec3 diffs[6];
	for (int a = 0; a < 6; a++) {
		diffs[a] = decodeD(diffVs[a/2], a - (a / 2) * 2);
	}

	//get diffuse lighting

	vec3 p2l = lightPos - wpos.xyz;
	float lp2l = length(p2l);
	float falloff = get_falloff(lp2l);
	vec3 p2li = p2l / lp2l;

	float not_shadow = 1;
	if (shadowStr > 0) {
		float pz = vec2depth(p2l);
		float sz = texture(shadowMap, -p2li.xyz).r;
		not_shadow = (sz >= pz - shadowBias) ? 1 : (1 - shadowStr);
	}

	vec3 emul = lightCol * lightStr * falloff * not_shadow;

	vec3 emits[6];
	for (int a = 0; a < 6; a++) {
		emits[a] = diffs[a] * emul * max(dot(p2li, normals[a]), 0);
	}

	//add to target value
	vec3 etx = texelFetch(emitTexX, coord).xyz;
	vec3 ety = texelFetch(emitTexY, coord).xyz;
	vec3 etz = texelFetch(emitTexZ, coord).xyz;

	outColorX.xyz = encodeE(
		max(emits[0], decodeE(etx, 0)),
		max(emits[1], decodeE(etx, 1))
	);
	outColorY.xyz = encodeE(
		max(emits[2], decodeE(ety, 0)),
		max(emits[3], decodeE(ety, 1))
	);
	outColorZ.xyz = encodeE(
		max(emits[4], decodeE(etz, 0)),
		max(emits[5], decodeE(etz, 1))
	);
}
)";
}