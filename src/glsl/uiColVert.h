#pragma once
namespace glsl {
    const char uiColVert[] = R"(
layout(location = 0) in vec3 pos;

void main(){
	gl_Position.xyz = pos;
	gl_Position.w = 1.0;
}
)";
}