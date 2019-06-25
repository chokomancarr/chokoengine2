#pragma once
namespace glsl {
    const char presumGGX[] = R"(
in vec2 uv;

uniform sampler2D mainTex;
uniform float rough;
uniform vec2 screenSize;
uniform samplerBuffer noise;
uniform int samples;

out vec4 fragCol;

vec2 xyz2uv(vec3 r) {
	vec2 refla = normalize(-vec2(dir.x, dir.z));
    float cx = acos(refla.x)/(3.14159 * 2);
    cx = mix(1-cx, cx, sign(refla.y) * 0.5 + 0.5);
    float sy = asin(dir.y)/(3.14159);
	return vec2(cx, sy + 0.5);
}

vec3 uv2xyz(vec2 s) {
	vec3 r;
	r.x = cos(s.x * 2 * 3.14159);
	r.z = sin(s.x * 2 * 3.14159);
	r.y = sin((s.y - 0.5) * 3.14159);
	r.xz *= sqrt(1 - r.y * r.y);
}

vec3 sampleGGX(vec2 rnd, vec3 nrm) {
	float a = rough * rough;
	float phi = 2 * 3.14159 * rnd.x;
	float cth = sqrt((1 - rnd.y) / (1 + (a * a - 1) * rnd.y));
	float sth = sqrt(1 - cth * cth);

	vec3 H;
	H.x = sth * cos(phi);
	H.y = sth * sin(phi);
	H.z = cth;

	vec3 up = abs(nrm.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 tx = normalize(cross(up, nrm));
	vec3 ty = cross(nrm, tx);

	return tx * H.x + ty * H.y + nrm * H.z;
}

void main () {
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 view = uv2xyz(uv);
	
	fragCol = vec4(0,0,0,0);
	for (int a = 0; a < samples; ++a) {
		vec2 rnd = texelFetch(noise, a);
		vec3 hv = sampleGGX(rnd, view);

		float3 lht = 2 * dot(view, hv) * hv - view;

		float ndl = saturate(dot(view, lht));
		if (ndl > 0) {
			fragCol += texture(mainTex, xyz2uv(lht)) * ndl;
			weight += ndl;
		}
	}
	fragCol /= weight;
}
)";
}