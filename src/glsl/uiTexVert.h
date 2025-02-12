#pragma once
namespace glsl {
	const char uiTexVert[] = R"(
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

out vec2 UV;

void main(){
	gl_Position.xyz = pos;
	gl_Position.w = 1.0;
	UV = uv;
}
)";
}