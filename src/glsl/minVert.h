#pragma once
namespace glsl {
    const char minVert[] = R"(
void main() {
	float y = -1;
	if (gl_VertexID > 1 && gl_VertexID < 5) y = 1;
	gl_Position = vec4(mod(gl_VertexID, 2)*2-1, y, 0.5, 1.0);
}
)";
}