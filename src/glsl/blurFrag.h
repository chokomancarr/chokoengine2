#pragma once
namespace glsl {
    const char blurFrag[] = R"(
in vec2 uv;

uniform sampler2D mainTex;
uniform float mul;
uniform vec2 screenSize;
uniform bool isY;

out vec4 fragCol;

void main () {
	vec2 uv = gl_FragCoord.xy / screenSize;
	float kernel[21] = float[]( 0.011, 0.0164, 0.023, 0.031, 0.04, 0.05, 0.06, 0.07, 0.076, 0.08, 0.0852, 0.08, 0.076, 0.07, 0.06, 0.05, 0.04, 0.031, 0.023, 0.0164, 0.011 );
	
	fragCol = vec4(0,0,0,0);
	for (int a = 0; a < 21; ++a) {
		int xx = a-10;
		vec2 uv2;
		if (isY) uv2 = uv + vec2(0, xx*mul/screenSize.y);
		else uv2 = uv + vec2(xx*mul/screenSize.x, 0);
		fragCol += (texture(mainTex, uv2) * kernel[a]);
	}
}
)";
}