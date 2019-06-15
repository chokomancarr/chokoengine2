#pragma once
namespace glsl {
    const char uiColFrag[] = R"(
uniform vec4 col;

out vec4 color;

void main(){
	color = col;
}
)";
}