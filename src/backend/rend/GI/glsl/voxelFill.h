#pragma once
namespace glsl {
	const char voxelFillVert[] = R"(
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 3) in vec2 uv;

uniform mat4 _M;
uniform mat4 _MVP;

out vec3 v2g_uvw;
out vec3 v2g_nrm;
out vec2 v2g_uv;

void main() {
	vec4 p = _MVP * vec4(pos, 1);
	v2g_uvw = p.xyz / p.w;
	v2g_nrm = (_M * vec4(nrm, 0)).xyz;
	v2g_uv = uv;
}
)";

	const char voxelFillGeom[] = R"(
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3[] v2g_uvw;
in vec3[] v2g_nrm;
in vec2[] v2g_uv;

uniform int layerCount;

out vec3 g2f_uvw;
out vec3 g2f_nrm;
out vec2 g2f_uv;

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
	g2f_uv = v2g_uv[0];
	EmitVertex();
	gl_Position = vec4(p2, 1);
	g2f_uvw = (v2g_uvw[1] * 0.5 + 0.5) * layerCount;
	g2f_nrm = v2g_nrm[1];
	g2f_uv = v2g_uv[1];
	EmitVertex();
	gl_Position = vec4(p3, 1);
	g2f_uvw = (v2g_uvw[2] * 0.5 + 0.5) * layerCount;
	g2f_nrm = v2g_nrm[2];
	g2f_uv = v2g_uv[2];
	EmitVertex();
	EndPrimitive();
}
)";

	const char voxelFillFrag[] = R"(
#version 420 core

in vec3 g2f_uvw;
in vec3 g2f_nrm;
in vec2 g2f_uv;

#ifdef emit_color
	#define has_emit
#else
	#ifdef emit_texture
		#define has_emit
	#endif
#endif

uniform vec3 diffCol;
#ifdef diffuse_texture
	uniform sampler2D diffTex;
#endif

#ifdef has_emit
	uniform float emitStr;
	#ifdef emit_color
		uniform vec3 emitCol;
	#endif
	#ifdef emit_texture
		uniform sampler2D emitTex;
	#endif
#endif

uniform layout(binding=0, rgba32f) writeonly image3D occluTexW;
uniform layout(binding=1, rgba32f) writeonly image3D diffTexWx;
uniform layout(binding=2, rgba32f) writeonly image3D diffTexWy;
uniform layout(binding=3, rgba32f) writeonly image3D diffTexWz;
uniform layout(binding=4, rgba32f) writeonly image3D emitTexWx;
uniform layout(binding=5, rgba32f) writeonly image3D emitTexWy;
uniform layout(binding=6, rgba32f) writeonly image3D emitTexWz;

float encode(vec2 v) {
	uint u1 = uint(v.x * 10000);
	uint u2 = uint(v.y * 10000);
	uint ru = uint(u1 << 16u) + u2;
	return uintBitsToFloat(ru);
}

vec3 encodeD(vec3 e, vec2 n) {
	vec3 e0 = e * n.x;
	vec3 e1 = e * n.y;

	return vec3(
		uintBitsToFloat((uint(e0.x * 10000) << 16u) + uint(e1.x * 10000)),
		uintBitsToFloat((uint(e0.y * 10000) << 16u) + uint(e1.y * 10000)),
		uintBitsToFloat((uint(e0.z * 10000) << 16u) + uint(e1.z * 10000))
	);
}

vec3 encodeE(vec3 e, vec2 n) {
	vec3 e0 = e * n.x;
	vec3 e1 = e * n.y;

	return vec3(
		uintBitsToFloat(packHalf2x16(vec2(e0.x, e1.x))),
		uintBitsToFloat(packHalf2x16(vec2(e0.y, e1.y))),
		uintBitsToFloat(packHalf2x16(vec2(e0.z, e1.z)))
	);
}

void main() {
	ivec3 coord;
	coord.x = int(g2f_uvw.x);
	coord.y = int(g2f_uvw.y);
	coord.z = int(g2f_uvw.z);

	vec3 nrm = normalize(g2f_nrm);
	vec2 nx = vec2(max(-nrm.x, 0), max(nrm.x, 0));
	vec2 ny = vec2(max(-nrm.y, 0), max(nrm.y, 0));
	vec2 nz = vec2(max(-nrm.z, 0), max(nrm.z, 0));

	imageStore(occluTexW, coord, vec4(encode(nx), encode(ny), encode(nz), 1));

	vec3 diff = diffCol;
#ifdef diffuse_texture
	diff = texture(diffTex, g2f_uv).rgb;
#endif

	imageStore(diffTexWx, coord, vec4(encodeD(diff, nx), 1));
	imageStore(diffTexWy, coord, vec4(encodeD(diff, ny), 1));
	imageStore(diffTexWz, coord, vec4(encodeD(diff, nz), 1));

#ifdef has_emit
	vec3 emit = vec3(1, 1, 1) * emitStr;
	#ifdef emit_color
		emit *= emitCol;
	#endif
	#ifdef emit_texture
		emit *= texture(emitTex, g2f_uv).rgb;
	#endif

	imageStore(emitTexWx, coord, vec4(encodeE(emit, nx), 0));
	imageStore(emitTexWy, coord, vec4(encodeE(emit, ny), 0));
	imageStore(emitTexWz, coord, vec4(encodeE(emit, nz), 0));
#endif
}
)";
}