namespace glsl {
	const char uvinfoExpFrag[] = R"(
uniform vec2 reso;
uniform isampler2D uvinfo;
uniform samplerBuffer uvcoords;
uniform isamplerBuffer indices;

out ivec4 outColor;

float cross2(vec2 v1, vec2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

void main() {
	vec2 uv = gl_FragCoord.xy / reso;
	
	const int CNT = 8;
	int kernel[CNT*2] = int[]( 
		-1,  0,   1, 0,
		 0, -1,   0, 1,
		-1, -1,  -1, 1,
		 1, -1,   1, 1
	);

	ivec4 mc = texture(uvinfo, uv);
	if (mc.r > 0) {
		outColor = mc;
		return;
	}

	for (int a = 0; a < CNT; a++) {
		vec2 duv = vec2(kernel[a*2], kernel[a*2 + 1]);
		mc = texture(uvinfo, uv + duv / reso);
		if (mc.x > 0) {
			outColor = mc;
			return;

			ivec3 ind = texelFetch(indices, mc.x-1).xyz;

			//coords
			vec2 u1 = texelFetch(uvcoords, ind.x).xy;
			vec2 u2 = texelFetch(uvcoords, ind.y).xy;
			vec2 u3 = texelFetch(uvcoords, ind.z).xy;
			vec2 uc = (u1 + u2 + u3) / 3;

			vec2 dc1 = normalize(u1 - uc);
			vec2 dc2 = normalize(u2 - uc);
			vec2 dc3 = normalize(u3 - uc);

			vec2 cp = normalize(uv - uc);

			// -- handle edge cases --

			float cpdc1 = dot(cp, dc1);
			float cpdc2 = dot(cp, dc2);
			float cpdc3 = dot(cp, dc3);

			/*
			if (cpdc1 > 0.999) {
				outColor.g = 0;
				outColor.b = 0;
				return;
			}
			if (cpdc2 > 0.999) {
				outColor.g = 1;
				outColor.b = 0;
				return;
			}
			if (cpdc3 > 0.999) {
				outColor.g = 2;
				outColor.b = 0;
				return;
			}
			*/

			// -- get edge lerps --

			float cpxu1 = cross2(cp, dc1);
			float cpxu2 = cross2(cp, dc2);
			float cpxu3 = cross2(cp, dc3);

			if (cpxu1 * cpxu2 > 0) {
				float a1 = acos(cpdc1);
				float a2 = acos(cpdc2);
				outColor.g = 0;
				outColor.b = int((a1 / (a1 + a2)) * 100);
			}
			else if (cpxu2 * cpxu3 > 0) {
				float a1 = acos(cpdc2);
				float a2 = acos(cpdc3);
				outColor.g = 1;
				outColor.b = int((a1 / (a1 + a2)) * 100);
			}
			else {
				float a1 = acos(cpdc3);
				float a2 = acos(cpdc1);
				outColor.g = 2;
				outColor.b = int((a1 / (a1 + a2)) * 100);
			}

			return;
		}
	}
}
)";
}