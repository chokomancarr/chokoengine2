namespace glsl {
	const char padTextureFrag[] = R"(
uniform vec2 reso;
uniform sampler2D colTex;
uniform sampler2D jmpTex;
uniform isampler2D idTex;

out vec4 outColor;

void main() {
	vec2 uv = gl_FragCoord.xy;
	vec2 uvr = uv / reso;

	ivec4 id = texture(idTex, uvr);
	if (id.x > 0) {
		outColor = texture(colTex, uvr);
		return;
	}

	vec4 jx = texture(jmpTex, uvr);
	if (jx.x >= 0) { //jump here
		outColor = texture(colTex, jx.xy);
		return;
	}
	
	const int CNT = 8;
	const int kernel[CNT*2] = int[]( 
		-1,  0,   1, 0,
		 0, -1,   0, 1,
		-1, -1,  -1, 1,
		 1, -1,   1, 1
	);
	for (int a = 0; a < CNT; a++) {
		vec2 duv = vec2(kernel[a*2], kernel[a*2 + 1]);
		vec2 uv2 = uvr + duv / reso;
		id = texture(idTex, uv2);
		if (id.x > 0) {
			outColor = texture(colTex, uv2);
			return;
		}
	}
	outColor = vec4(0, 0, 0, 1);
}
)";
}