#pragma once
#include "inc/random.h"

namespace glsl {
    const std::string presumGGX = R"(
in vec2 uv;

uniform sampler2D mainTex;
uniform float rough;
uniform vec2 screenSize;
uniform int samples;
uniform int level;

out vec4 fragCol;

)" + inc::rand + R"(

vec2 xyz2uv(vec3 dir) {
	vec2 refla = normalize(vec2(dir.x, dir.z));
    float cx = acos(refla.x)/(3.14159 * 2);
    cx = mix(1-cx, cx, sign(refla.y) * 0.5 + 0.5);
    float sy = asin(dir.y)/(3.14159);
	return vec2(cx, sy + 0.5);
}

vec3 uv2xyz(vec2 s) {
	vec3 r;
	r.y = sin((s.y - 0.5) * 3.14159);
	r.x = cos(s.x * 2 * 3.14159);
	r.z = sin(s.x * 2 * 3.14159);
	r.xz *= sqrt(1 - r.y * r.y);
	return r;
}

vec3 sampleGGX(vec2 rnd, vec3 nrm, vec3 tx, vec3 ty) {
	float a = rough * rough;
	float phi = 2 * 3.14159 * rnd.x;
	float cth = sqrt((1 - rnd.y) / (1 + (a * a - 1) * rnd.y));
	float sth = sqrt(1 - cth * cth);

	return (tx * cos(phi) + ty * sin(phi)) * sth + nrm * cth;
}

void main () {
	vec2 uv = gl_FragCoord.xy / screenSize;
	vec3 view = uv2xyz(uv);

	vec3 up = abs(view.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 tx = normalize(cross(up, view));
	vec3 ty = cross(view, tx);
	
	fragCol = vec4(0,0,0,0);
	vec2 rnd = vec2(0, rand(uv));
	float weight = 0;
	for (int a = 0; a < samples; ++a) {
		rnd.x = rand(rnd.y);
		rnd.y = rand(rnd.x);

		vec3 hv = sampleGGX(rnd, view, tx, ty);
		vec3 lht = normalize(2 * dot(view, hv) * hv - view);

		float ndl = clamp(dot(view, lht), 0, 1);
		if (ndl > 0) {
			fragCol += textureLod(mainTex, xyz2uv(lht), level) * ndl;
			weight += ndl;
		}
	}
	fragCol /= weight;
}
)";
}