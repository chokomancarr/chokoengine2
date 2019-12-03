namespace glsl {
	const char surfBlurFrag[] = R"(
uniform vec2 sres;
uniform vec2 reso;
uniform sampler2D colTex;
//tri id (x), edge id (y), edge t (z)
uniform isampler2D idTex;
//px py t1 t2
uniform sampler2D jmpTex;
//model coords
uniform samplerBuffer posBuf;
//for each tri [i(xy) j(xy)]
uniform samplerBuffer edatBuf;
//for each tri [for each edge [i1, i2, it]]
uniform isamplerBuffer iconBuf;
uniform vec2 dir0;

out vec4 outColor;

const float rad2deg = 180 / 3.14159;

const int tolerance = 2;

const int BLUR_CNT = 10;

const float kernel[11] = float[](
	0.100346, 0.097274, 0.088613, 0.075856,
	0.061021, 0.046128, 0.032768, 0.021874,
	0.013722, 0.008089, 0.004481 );

float length2(vec3 r) {
	return dot(r, r);
}

vec2 rebase2(vec2 v, vec2 i, vec2 j) {
	mat2 m;
	m[0] = i; m[1] = j;
	return inverse(m) * v;
}

vec2 rebase3(vec3 v, vec3 i, vec3 j) {
	vec3 k = cross(i, j);
	mat3 m;
	m[0] = i; m[1] = j; m[2] = k;
	return (inverse(m) * v).xy;
}

vec3 orient(vec3 r, vec3 x, vec3 t) {
	x = normalize(x);
	t = normalize(t);
	float th = dot(r, x);
	float lt = sqrt(length2(r) - th * th);
	return th * x + lt * t;
}

vec4 sample(
		vec2 dr, //direction
		ivec2 px, //first pixel
		vec2 uv, vec2 dreso) {

	vec2 poso = uv;

	vec2 drr = dr * dreso;
	vec2 pos = uv + dr;
	int tid = px.x - 1;
	int eid = px.y;

	vec4 col = vec4(0, 0, 0, 0);

	for (int a = 0; a < BLUR_CNT; a++) {
		vec4 nxt = texture(jmpTex, pos * dreso);
		ivec2 npx;
		if (nxt.x >= 0) { //jump here
			pos = nxt.xy * reso;
			npx.x = int(nxt.z);
			npx.y = int(nxt.w);
		}
		else {
			npx = texture(idTex, pos * dreso).xy;
			if (npx.x <= 0) { //not triangle, go back
				pos = poso;
				npx = texture(idTex, pos * dreso).xy;
			}
		}
		int tid2 = npx.x - 1;
		if (tid2 < 0) tid2 = tid;
		if (tid != tid2) {
			vec4 ed = texelFetch(edatBuf, tid * 3);
			vec2 ts1 = rebase2(dr, ed.xy, ed.zw);

			vec3 t1i = texelFetch(edatBuf, tid * 3 + 1).xyz;
			vec3 t1j = texelFetch(edatBuf, tid * 3 + 2).xyz;

			vec3 t1k = t1i;
			if (eid == 2) t1k = t1j;
			else if (eid == 1) t1k = normalize(t1j - t1i);
			vec3 t2k = -t1k;

			vec3 ws = normalize(t1i * ts1.x + t1j * ts1.y);

			vec3 t2i = texelFetch(edatBuf, tid2 * 3 + 1).xyz;
			vec3 t2j = texelFetch(edatBuf, tid2 * 3 + 2).xyz;

			ivec3 icon = texelFetch(iconBuf, tid * 3 + eid).xyz;
			vec3 t23 = texelFetch(posBuf, icon.z).xyz - texelFetch(posBuf, icon.y).xyz;

			vec3 t2n = cross(t2k, t23);
			vec3 t2t = normalize(cross(t2n, t2k));

			vec3 ws2 = orient(ws, t2k, t2t);
			vec2 ts2 = rebase3(ws2, t2i, t2j);

			//new coords
			tid = tid2;
			vec4 tcd = texelFetch(edatBuf, tid * 3);
			dr = tcd.xy * ts2.x + tcd.zw * ts2.y;
			dr = normalize(dr);
			drr = dr * dreso;

			//col = vec4(dr, icon.z, 1);//+= texture(colTex, pos * dreso);
		}
		eid = npx.y;
		poso = pos;
		pos += dr;

		col += texture(colTex, pos * dreso) * kernel[a+1];
	}
	return col;
}

void main() {
	vec2 uv = gl_FragCoord.xy;
	vec2 dreso = 1.0 / reso;
	vec2 uvr = uv / sres;
	uv = uvr * reso;

	//this color
	vec4 col = texture(colTex, uvr);

	ivec2 px;
	//jump?
	vec4 jx = texture(jmpTex, uvr);
	if (jx.x >= 0) {
		uvr = jx.xy;
		uv = uvr * reso;
		px.x = int(jx.z);
		px.y = int(jx.w);
	}
	else {
		//in triangle?
		px = texture(idTex, uvr).xy;
		if (px.x == 0) {
			outColor = col;
			//outColor = vec4(0, 0, 0, 0);
			return;
		}
	}

	outColor = col * kernel[0] +
		sample(dir0, px, uv, dreso) + 
		sample(-dir0, px, uv, dreso);
}
)";
}