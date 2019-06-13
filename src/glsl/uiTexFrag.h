#pragma once
namespace glsl {
    const char uiTexFrag[] = R"(
in vec2 UV;

uniform sampler2D tex;
uniform vec4 col;
uniform float level;

out vec4 color;

void main(){
	color = textureLod(tex, UV, level)*col;
}
)";
}