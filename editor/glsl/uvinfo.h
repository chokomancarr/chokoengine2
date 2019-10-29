namespace glsl {
	const char uvinfoVert[] = R"(
uniform samplerBuffer uvTex;
uniform isamplerBuffer elemTex;

out vec3 v2f_tri;

void main() {
	int i = int(mod(gl_VertexID, 3));
	v2f_tri.x = (i == 0) ? 1.0 : 0.0;
	v2f_tri.y = (i == 1) ? 1.0 : 0.0;
	v2f_tri.z = (i == 2) ? 1.0 : 0.0;

	int e = texelFetch(elemTex, gl_VertexID / 3)[i];
	gl_Position = vec4(texelFetch(uvTex, e).xy * 2.0 - 1.0, 0, 1);
}
)";

	const char uvinfoFrag[] = R"(
in vec3 v2f_tri;

out ivec4 outColor;

void main() {
	outColor.r = gl_PrimitiveID + 1;
	if ((v2f_tri.x < v2f_tri.y) && (v2f_tri.x < v2f_tri.z)) {
		outColor.g = 1;
		outColor.b = int(v2f_tri.z * 100);
	}
	else if ((v2f_tri.y < v2f_tri.x) && (v2f_tri.y < v2f_tri.z)) {
		outColor.g = 2;
		outColor.b = int(v2f_tri.x * 100);
	}
	else {
		outColor.g = 0;
		outColor.b = int(v2f_tri.y * 100);
	}
	outColor.a = 1;
}
)";
}