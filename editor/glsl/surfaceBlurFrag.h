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
//index to element
uniform isamplerBuffer indBuf;
//for each tri [i(xy) j(xy)]
uniform samplerBuffer edatBuf;
//for each tri [for each edge [i1, i2, it]]
uniform isamplerBuffer iconBuf;
//for each tri [for each edge [uv1(xy) uv2(zw) or -1 if no connection]]
uniform samplerBuffer conBuf;
uniform vec2 dir0;
uniform int blurcnt;

out vec4 outColor;

const float rad2deg = 180 / 3.14159;

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
	float th = dot(r, x);
	float lt = sqrt(length2(r) - th * th);
	return th * x + lt * t;
}

vec4 sample(
		vec2 dr, //direction
		ivec4 px, //first pixel
		vec2 uv, vec2 dreso, inout int n) {
	vec2 drr = dr * dreso;
	vec2 pos = uv + dr;
	int tid = px.x - 1;
	int eid = px.y;

	vec4 col = vec4(0, 0, 0, 0);

	for (int a = 0; a < blurcnt; a++) {
		vec2 nxt = texture(jmpTex, pos * dreso).xy;
		if (nxt.x >= 0) { //jump here
			pos = nxt.xy * reso;
		}
		ivec4 npx = texture(idTex, pos * dreso);
		int tid2 = npx.x - 1;
		if (tid2 < 0) { //not triangle, finish
			break;
		}
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
			//dr = tcd.xy * ts2.x + tcd.zw * ts2.y;
			//dr = normalize(dr);
			drr = dr * dreso;
		}
		eid = npx.y;
		pos += dr;

		col += texture(colTex, pos * dreso);
		n += 1;
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
	//get current triangle
	ivec4 px = texture(idTex, uvr);
	//not in triangle, end
	if (px.x == 0) {
		outColor = col * 0.5;
		return;
	}
	int n = 1;
	col += sample(dir0, px, uv, dreso, n) + sample(-dir0, px, uv, dreso, n);
	outColor = col / n;
}
)";
}