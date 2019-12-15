namespace glsl {
	const char surfBlurFrag[] = R"(
uniform vec2 reso;
uniform sampler2D colTex;
//px py t1 t2
uniform isampler2D infoTex;
//matrices
uniform samplerBuffer matTex;
//center, vert vectors
uniform isamplerBuffer vecTex;
//scale factor
uniform samplerBuffer sclTex;
uniform vec2 dir0;

out vec4 outColor;

const float pi2 = 3.14159 * 2;

const int BLUR_CNT = 10;

const float kernel[11] = float[](
	0.082607, 0.080977, 0.076276,
	0.069041, 0.060049, 0.050187,
	0.040306, 0.031105, 0.023066,
	0.016436, 0.011254 );
/*
int get_eid(vec2 p, int tid) {
	ivec4 angi = texelFetch(angTex, tid);

	vec2 c = angi.xy * 0.00001;
	float t0 = (angi.z >> 10) * 0.00001;
	float t1 = (angi.w >> 10) * 0.00001;
	const int mask = (1 << 10) - 1;
	float t2 = (((angi.z & mask) << 10) | (angi.w & mask)) * 0.00001;

	vec2 dp = normalize(p - c);
	float tp = acos(dp.x);
	tp = (dp.y > 0) ? tp : pi2 - tp;

	tp -= t0;
	tp = (tp < 0) ? pi2 + tp : tp;

	if (tp > t2) return 2;
	else if (tp > t1) return 1;
	else return 0;
}
*/

vec2 split16(int i) {
	return vec2(
		i >> 16,
		i & ((1 << 16) - 1)
	) * 0.00005;
}
vec2 split16n(int i) {
	return (vec2(
		i >> 16,
		i & ((1 << 16) - 1)
	) - 10000) * 0.0001;
}

float cross2(vec2 v1, vec2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

int get_eid(vec2 p, int tid) {
	ivec4 data = texelFetch(vecTex, tid);

	vec2 c = split16(data.x);
	vec2 p1 = split16n(data.y);
	vec2 p2 = split16n(data.z);
	vec2 p3 = split16n(data.w);
	vec2 cp = p - c;

	float cpxu1 = cross2(cp, p1);
	float cpxu2 = cross2(cp, p2);
	float cpxu3 = cross2(cp, p3);

	if (cpxu1 * cpxu2 <= 0 && dot(cp, p1 + p2) > 0) {
		return 0;
	}
	else if (cpxu2 * cpxu3 <= 0 && dot(cp, p2 + p3) > 0) {
		return 1;
	}
	else return 2;
}

vec4 sample(vec2 dr, //direction
			ivec4 npx, //first data
			vec2 uv, vec2 dreso) {

	vec2 poso = uv;

	vec2 drr = dr * dreso;
	vec2 pos = uv;
	int tid = npx.z - 1;

	ivec4 npxo;

	vec4 col = vec4(0, 0, 0, 0);

	float cost = 1;
	float hcost = cost * 0.5;
	float hp = 1;
	int a = 0;
	
	while (a < BLUR_CNT) {
		for (int k = 0; k < 5 && hp >= hcost; k++) {
			pos += dr;
			int eid = get_eid(pos * dreso, tid);
			npxo = npx;
			npx = texture(infoTex, pos * dreso);
			if (npx.x > 0) { //jump here
				pos = (npx.xy - 1) / 100000.0 * reso;
			}
			else if (npx.z == 0) { //not triangle, go back
				pos = poso;
				npx = npxo;
			}
			int tid2 = npx.z - 1;
			if (tid != tid2) {
				vec4 rmatv = texelFetch(matTex, tid * 3 + eid);
				mat2 rmat = mat2(rmatv.xy, rmatv.zw);
				dr = rmat * dr;

				//new coords
				tid = tid2;
				float ldr = 1.0 / length(dr);
				cost *= ldr;
				dr *= ldr;
				hcost = cost * 0.5;
				drr = dr * dreso;
			}
			poso = pos;
			hp -= cost;
		}
		vec4 rcol = texture(colTex, pos * dreso);
		while (hp < hcost && a < BLUR_CNT) {
			a += 1;
			col += rcol * kernel[a];
			hp += 1;
		}
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
	else if (info.z == 0) { //not triangle, end
		outColor = col;
		return;
	}

	vec2 scl = texelFetch(sclTex, info.z - 1).xy;

	vec2 dir = dir0 * scl;

	outColor = col * kernel[0] +
		sample(dir, info, uv, dreso) + 
		sample(-dir, info, uv, dreso);
}
)";
}