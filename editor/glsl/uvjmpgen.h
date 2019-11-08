namespace glsl {
	const char uvjmpgenFrag[] = R"(
uniform vec2 reso;
//uv data: tri id (x), edge id (y), edge t (z)
uniform isampler2D uvinfo;
//edge data: for each tri [for each edge [uv1(xy) uv2(zw) or -1 if no connection]]
uniform samplerBuffer edgeData;

out vec4 outColor;

const float rad2deg = 180 / 3.14159;

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

	for (int a = 0; a < CNT; a++) { //pixel beside triangle
		vec2 duv = vec2(kernel[a*2], kernel[a*2 + 1]);
		info = smp(uv + duv);
		int tid = (info.x);
		if (tid == 0) continue; //not triangle
		int eid = (info.y);
		vec4 data = texelFetch(edgeData, ((tid-1) * 3 + eid) * 2);
		if (data.x < 0) { //no connection
			outColor = vec4(-1, 0, 0, 0);
			return;
		}
		outColor.xy = mix(data.xy, data.zw, info.z * 0.01);
		vec2 dir = normalize(data.zw - data.xy);
		outColor.xy -= duv / reso;
		outColor.z = info.z * 0.01;
		return;
	}
	
	outColor = vec4(-1, 0, 0, 0); //blank
}
)";
}