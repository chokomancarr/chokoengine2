namespace glsl {
	const char padTextureFrag[] = R"(
uniform vec2 reso;
uniform sampler2D colTex;
//px py t1 t2
uniform sampler2D jmpTex;

out vec4 outColor;

void main() {
	vec2 uv = gl_FragCoord.xy;
	vec2 uvr = uv / reso;

	outColor = texture(colTex, uvr);
	
	vec4 jx = texture(jmpTex, uvr);
	if (jx.x >= 0) { //jump here
		outColor = texture(colTex, jx.xy);
	}
}
)";
}