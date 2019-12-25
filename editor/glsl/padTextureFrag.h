namespace glsl {
	const char padTextureFrag[] = R"(
uniform vec2 reso;
uniform sampler2D colTex;
uniform isampler2D infoTex;

out vec4 outColor;

void main() {
	vec2 uv = gl_FragCoord.xy;
	vec2 uvr = uv / reso;

	ivec4 info = texture(infoTex, uvr);
	outColor = texture(colTex, uvr);
	if (info.x == 0 && info.z > 0) { //in triangle
		return;
	}

	if (info.x > 0) { //jump here
		outColor = texture(colTex, (info.xy - 1) / 100000.0);
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
		info = texture(infoTex, uv2);
		if (info.x == 0 && info.z > 0) {
			outColor = texture(colTex, uv2);
			return;
		}
	}
}
)";
}