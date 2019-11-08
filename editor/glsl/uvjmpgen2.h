namespace glsl {
	const char uvjmpgenFrag[] = R"(
uniform vec2 reso;
uniform isamplerBuffer indices;
uniform samplerBuffer uvcoords;
//uv data: tri id (x), edge id (y), edge t (z)
uniform isampler2D uvinfo;
//edge data: for each tri [for each edge [uv1(xy) uv2(zw) or -1 if no connection]]
uniform samplerBuffer edgeData;

out vec4 outColor;

const float rad2deg = 180 / 3.14159;

float cross2(vec2 v1, vec2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

ivec4 smp(vec2 v) {
	return texture(uvinfo, v / reso);
}

void main() {
	vec2 uv = gl_FragCoord.xy;
	
	const int CNT = 8;
	int kernel[CNT*2] = int[]( 
		-1,  0,   1, 0,
		 0, -1,   0, 1,
		-1, -1,  -1, 1,
		 1, -1,   1, 1
	);

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
				vec4 tarv = texelFetch(edgeData, (info.x-1)*6);
				if (tarv.x < 0) { //no connection
					outColor = vec4(-1, 0, 0, 0);
					return;
				}
				float a1 = acos(cpdc1);
				float a2 = acos(cpdc2);
				float lp = (a1 / (a1 + a2));
				outColor.rg = mix(tarv.xy, tarv.zw, lp);
				outColor.b = lp;
			}
			else if (cpxu2 * cpxu3 < 0 && dot(cp, dc23) > 0) {
				vec4 tarv = texelFetch(edgeData, (info.x-1)*6 + 2);
				if (tarv.x < 0) { //no connection
					outColor = vec4(-1, 0, 0, 0);
					return;
				}
				float a1 = acos(cpdc2);
				float a2 = acos(cpdc3);
				float lp = (a1 / (a1 + a2));
				outColor.rg = mix(tarv.xy, tarv.zw, lp);
				outColor.b = lp;
			}
			else {
				vec4 tarv = texelFetch(edgeData, (info.x-1)*6 + 4);
				if (tarv.x < 0) { //no connection
					outColor = vec4(-1, 0, 0, 0);
					return;
				}
				float a1 = acos(cpdc3);
				float a2 = acos(cpdc1);
				float lp = (a1 / (a1 + a2));
				outColor.rg = mix(tarv.xy, tarv.zw, lp);
				outColor.b = lp;
			}
			return;
		}
	}

	outColor = vec4(-1, 0, 0, 0); //blank
}
)";
}