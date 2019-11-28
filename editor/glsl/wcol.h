#pragma once
namespace glsl {
	const char wColVert[] = R"(
layout(location = 0) in vec3 pos;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(pos, 1);
}
)";
    const char wColFrag[] = R"(
uniform vec4 col;

out vec4 color;

void main(){
	color = col;
}
)";
}