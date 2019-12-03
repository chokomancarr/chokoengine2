namespace glsl {
	const char uvjmpgenFrag[] = R"(
uniform vec2 reso;
uniform isamplerBuffer indices;
uniform samplerBuffer uvcoords;
//uv data: tri id (x), edge id (y), edge t (z)
uniform isampler2D uvinfo;
uniform isamplerBuffer iconData;

out ivec4 outColor;

const float rad2deg = 180 / 3.14159;
	
const int CNT = 8;
const int kernel[CNT*2] = int[]( 
	-1,  0,   1, 0,
	 0, -1,   0, 1,
	-1, -1,  -1, 1,
	 1, -1,   1, 1
);

float cross2(vec2 v1, vec2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

float lineline(vec2 p1, vec2 p2, vec2 p3, vec2 p4) {
	float n1 = (p1.x - p3.x) * (p3.y - p4.y);
	float n2 = (p1.y - p3.y) * (p3.x - p4.x);
	float d1 = (p1.x - p2.x) * (p3.y - p4.y);
	float d2 = (p1.y - p2.y) * (p3.x - p4.x);
	return (n1 - n2) / (d1 - d2);
}

ivec4 smp(vec2 v) {
	return texture(uvinfo, v / reso);
}
 
vec4 get_tarv(ivec3 icon) {
	vec2 v1 = texelFetch(uvcoords, icon.x).xy;
	vec2 v2 = texelFetch(uvcoords, icon.y).xy;
	vec2 v3 = texelFetch(uvcoords, icon.z).xy;
	vec2 vc = (v1 + v2 + v3) * 0.333333;
	vec2 dreso = 1.0 / reso;
	vec2 vc1 = normalize(vc - v1);
	vec2 vc2 = normalize(vc - v2);
	vec2 v12 = (v1 + v2) * 0.5;
	vec2 vc12 = normalize(vc - v12);
	return vec4(
		v1 + normalize(vc - v1) * dreso / dot(vc1, vc12),
		v2 + normalize(vc - v2) * dreso / dot(vc2, vc12)
	);
}

ivec4 get_output(int id, int vid, int eid, vec2 p, vec2 v1, vec2 v2, vec2 vc) {
	ivec4 icon = texelFetch(iconData, id);
	if (icon.x < 0) { //no connection
		return ivec4(0, 0, 0, 0);
	}

	vec2 tar1 = texelFetch(uvcoords, icon.x).xy;
	vec2 tar2 = texelFetch(uvcoords, icon.y).xy;
	
	float lp = lineline(v1, v2, vc, p);

	vec2 lr = mix(tar1, tar2, lp);
	
	return ivec4(
		int(100000 * lr.x) + 1, int(100000 * lr.y) + 1,
		icon.w / 3 + 1, icon.w - (icon.w / 3) * 3);
}

void main() {
	vec2 uv = gl_FragCoord.xy;

	ivec4 info = smp(uv);
	if (info.x > 0) { //pixel inside triangle
		outColor = ivec4(0, 0, info.x, info.y);
		return;
	}

	for (int a = 0; a < CNT; a++) {
		vec2 duv = vec2(kernel[a*2], kernel[a*2 + 1]);
		info = smp(uv + duv);
		if (info.x > 0) { //pixel beside triangle

			ivec3 ind = texelFetch(indices, info.x-1).xyz;

			//coords
			vec2 u1 = texelFetch(uvcoords, ind.x).xy;
			vec2 u2 = texelFetch(uvcoords, ind.y).xy;
			vec2 u3 = texelFetch(uvcoords, ind.z).xy;
			vec2 uc = (u1 + u2 + u3) / 3;

			vec2 dc1 = normalize(u1 - uc);
			vec2 dc2 = normalize(u2 - uc);
			vec2 dc3 = normalize(u3 - uc);

			vec2 dc12 = dc1 + dc2;
			vec2 dc23 = dc2 + dc3;
			vec2 dc31 = dc3 + dc1;

			vec2 uvr = uv / reso;
			vec2 cp = normalize(uvr - uc);

			float cpdc1 = dot(cp, dc1);
			float cpdc2 = dot(cp, dc2);
			float cpdc3 = dot(cp, dc3);
			float c1dc2 = dot(dc1, dc2);
			float c2dc3 = dot(dc2, dc3);
			float c3dc1 = dot(dc3, dc1);

			// -- handle edge cases --

			// -- get edge lerps --

			float cpxu1 = cross2(cp, dc1);
			float cpxu2 = cross2(cp, dc2);
			float cpxu3 = cross2(cp, dc3);
			float u1xu2 = cross2(dc1, dc2);
			float u2xu3 = cross2(dc2, dc3);
			float u3xu1 = cross2(dc3, dc1);

			if (cpxu1 * cpxu2 < 0 && dot(cp, dc12) > 0) {
				outColor = get_output((info.x-1)*3, info.x, 0, uvr, u1, u2, uc);
			}
			else if (cpxu2 * cpxu3 < 0 && dot(cp, dc23) > 0) {
				outColor = get_output((info.x-1)*3 + 1, info.x, 1, uvr, u2, u3, uc);
			}
			else {
				outColor = get_output((info.x-1)*3 + 2, info.x, 2, uvr, u3, u1, uc);
			}
			return;
		}
	}

	outColor = ivec4(0, 0, 0, 0); //blank
}
)";
}