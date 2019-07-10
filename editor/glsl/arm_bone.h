#pragma once
namespace glsl {
	const char armBoneVert[] = R"(
in vec3 position;

uniform mat4 MVP;
uniform float len;

void main() {
	vec3 p3 = position * len;
    vec4 pos = vec4(p3, 1);
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