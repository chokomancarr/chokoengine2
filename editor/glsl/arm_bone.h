#pragma once
namespace glsl {
	const char armBoneVert[] = R"(
in vec3 position;
in float len;

uniform mat4 MVP;

void main() {
    vec4 pos = vec3(position * len, 1);
    gl_Position = MVP * pos;
}
)";
}