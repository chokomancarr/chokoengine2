namespace glsl {
	const char edgeDetFrag[] = R"(
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

out vec4 outColor;

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

	//return texture(colTex, (uv + dr) * dreso);

	vec2 poso = uv;

	vec2 drr = dr * dreso;
	vec2 pos = uv;
	int tid = npx.z - 1;

	ivec4 npxo;

	vec4 col = vec4(0, 0, 0, 0);

	float ldr = 1.0 / length(dr);
	dr *= ldr;
	float cost = ldr;
	float hcost = cost * 0.5;
	float hp = 1;
	int a = 0;
	

	for (int a = 0; a < 5 && hp >= hcost; a++) {
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
			
			ldr = 1.0 / length(dr);
			cost *= ldr;
			dr *= ldr;
			hcost = cost * 0.5;
			drr = dr * dreso;
		}
		poso = pos;
		hp -= cost;
	}
	
	return texture(colTex, pos * dreso);
}

const float dirs[16] = float[](
	-1,  1,   0,  1,   1,  1,
	-1,  0,            1,  0,
	-1, -1,   0, -1,   1, -1);

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

	outColor = col * 8;
	for (int a = 0; a < 8; a++) {
		outColor -= sample(vec2(dirs[a*2], dirs[a*2+1]) * scl, info, uv, dreso);
	}
	outColor.a = 1;
	outColor = max(outColor, 0.0);
}
)";
}