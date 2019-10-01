#pragma once
namespace glsl {
    const char presumGGXCube[] = R"(
in vec2 uv;

uniform samplerCube cubemap;
uniform float rough;
uniform vec2 screenSize;
uniform samplerBuffer noise;
uniform int samples;
uniform vec3 face;
uniform vec3 dirx;
uniform vec3 diry;
uniform float level;

out vec4 fragCol;

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
	float off = (gl_FragCoord.y + screenSize.y * gl_FragCoord.x) * 71;
	
	vec3 view = normalize(face + dirx * (uv.x * 2 - 1) + diry * (uv.y * 2 - 1));
	
	fragCol = vec4(0,0,0,0);
	float weight = 0;
	for (int a = 0; a < samples; ++a) {
		int ni = int(mod(off + a * 17, 65534));
		vec2 rnd = texelFetch(noise, ni).xy;

		vec3 hv = sampleGGX(rnd, view);
		vec3 lht = normalize(2 * dot(view, hv) * hv - view);

		float ndl = clamp(dot(view, lht), 0, 1);
		if (ndl > 0) {
			fragCol += textureLod(cubemap, lht, level) * ndl;
			weight += ndl;
		}
	}
	fragCol /= weight;
}
)";
}