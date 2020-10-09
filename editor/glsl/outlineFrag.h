#pragma once
namespace glsl {
	const char outlineFrag[] = R"(
uniform vec2 dscreenSize;
uniform isampler2D idtex;
uniform int tarid;
uniform int radius;
uniform vec4 color;

out vec4 fragCol;

void main () {
    vec2 uv = gl_FragCoord.xy * dscreenSize;
	fragCol = vec4(0);

	int myid = texture(idtex, uv).r;
    if (myid != tarid) {
		for (int a = -radius; a <= radius; a++) {
			for (int b = -radius; b <= radius; b++) {
				if (texture(idtex, uv + dscreenSize * vec2(a, b)).r == tarid) {
					fragCol = color;
					return;
				}
			}
		}
	}
}
)";
}