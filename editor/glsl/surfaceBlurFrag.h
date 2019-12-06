namespace glsl {
	const char surfBlurFrag[] = R"(
uniform vec2 reso;
uniform sampler2D colTex;
//px py t1 t2
uniform isampler2D infoTex;
//matrices
uniform samplerBuffer matTex;
uniform vec2 dir0;

out vec4 outColor;

const float rad2deg = 180 / 3.14159;

const int tolerance = 2;

const int BLUR_CNT = 10;

const float kernel[11] = float[](
	0.082607, 0.080977, 0.076276,
	0.069041, 0.060049, 0.050187,
	0.040306, 0.031105, 0.023066,
	0.016436, 0.011254 );

vec4 sample(
		vec2 dr, //direction
		ivec4 npx, //first data
		vec2 uv, vec2 dreso) {

	vec2 poso = uv;

	vec2 drr = dr * dreso;
	vec2 pos = uv;
	int tid = npx.z - 1;
	int eid = npx.w;

	ivec4 npxo;

	vec4 col = vec4(0, 0, 0, 0);

	for (int a = 0; a < BLUR_CNT; a++) {
		pos += dr;
		npxo = npx;
		npx = texture(infoTex, pos * dreso);
		if (npx.x > 0) { //jump here
			pos = (npx.xy - 1) / 100000.0 * reso;
		}
		else {
			if (npx.z == 0) { //not triangle, go back
				pos = poso;
				npx = npxo;
			}
		}
		int tid2 = npx.z - 1;
		if (tid != tid2) {
			vec4 rmatv = texelFetch(matTex, tid * 3 + eid);
			mat2 rmat = mat2(rmatv.xy, rmatv.zw);
			dr = rmat * dr;

			//new coords
			tid = tid2;
			dr = normalize(dr);
			drr = dr * dreso;
		}
		poso = pos;
		eid = npx.w;

		col += texture(colTex, pos * dreso) * kernel[a+1];
	}
	return col;
}

void main() {
	vec2 uv = gl_FragCoord.xy;
	vec2 dreso = 1.0 / reso;
	vec2 uvr = uv * dreso;

	//this color
	vec4 col = texture(colTex, uvr);

	ivec4 info = texture(infoTex, uvr);
	if (info.x > 0) { //jump here
		uvr = (info.xy - 1) / 100000.0;
		uv = uvr * reso;
	}
	else {
		if (info.z == 0) { //not triangle, end
			outColor = col;
			return;
		}
	}

	outColor = col * kernel[0] +
		sample(dir0, info, uv, dreso) + 
		sample(-dir0, info, uv, dreso);
}
)";
}