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

	const char armBoneFrag[] = R"(

uniform vec4 color;

out vec4 fragColor;

void main() {
	fragColor = color;
}
)";
}