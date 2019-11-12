namespace glsl {
	const char uvjmpgenFrag[] = R"(
uniform vec2 reso;
uniform isamplerBuffer indices;
uniform samplerBuffer uvcoords;
//uv data: tri id (x), edge id (y), edge t (z)
uniform isampler2D uvinfo;
uniform isamplerBuffer iconData;

out vec4 outColor;

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

vec4 get_output(int id, float c1, float c2) {
	ivec3 icon = texelFetch(iconData, id).xyz;
	if (icon.x < 0) { //no connection
		return vec4(-1, 0, 0, 0);
	}

	vec2 v1 = texelFetch(uvcoords, icon.x).xy;
	vec2 v2 = texelFetch(uvcoords, icon.y).xy;
	vec2 v3 = texelFetch(uvcoords, icon.z).xy;
	vec2 vc = (v1 + v2 + v3) * 0.333333;
	vec2 dreso = 1.0 / reso;
	vec2 vc1 = (vc - v1);
	float vc1l = length(vc1);
	vc1 /= vc1l;
	vec2 vc2 = normalize(vc - v2);
	vec2 v12 = (v1 + v2) * 0.5;
	vec2 vc12 = normalize(vc - v12);

	vec2 tarx = v1 + normalize(vc - v1) * dreso / dot(vc1, vc12);
	vec2 tary = v2 + normalize(vc - v2) * dreso / dot(vc2, vc12);

	float a1 = acos(c1);
	float a2 = acos(c2);
	vec2 v21 = normalize(v2 - v1);
	float th1 = acos(dot(vc1, v12));
	float th2 = 3.14159 - th1 - a1;

	float lp = (a1 / (a1 + a2));
	return vec4(
		mix(tarx, tary, lp),
		lp, 1);
}

void main() {
	vec2 uv = gl_FragCoord.xy;

	ivec4 info = smp(uv);
	if (info.x > 0) { //pixel inside triangle
		outColor = vec4(-1, info.x * 0.3, 0.1 + info.y * 0.3, 0);
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

			vec2 cp = normalize(uv / reso - uc);

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
				outColor = get_output((info.x-1)*3, cpdc1, cpdc2);
			}
			else if (cpxu2 * cpxu3 < 0 && dot(cp, dc23) > 0) {
				outColor = get_output((info.x-1)*3 + 1, cpdc2, cpdc3);
			}
			else {
				outColor = get_output((info.x-1)*3 + 2, cpdc3, cpdc1);
			}
			return;
		}
	}

	outColor = vec4(-1, 0, 0, 0); //blank
}
)";
}