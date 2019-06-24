in vec3 v2f_normal;

uniform vec4 col;

layout (location=0) out vec4 outColor; //rgba
layout (location=1) out vec4 outNormal; //xyz []
layout (location=2) out vec4 outSpec; //spec gloss
layout (location=3) out vec4 outEmi; //emi occlu

void main() {
    outColor = col;
    outNormal.xyz = v2f_normal;
}
