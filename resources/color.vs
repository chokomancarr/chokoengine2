layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 tangent;
layout(location=3) in vec2 texCoord;

uniform mat4 _MV;
uniform mat4 _P;
uniform mat4 _MVP;

out vec3 v2f_normal;

void main(){
	gl_Position = _MVP*vec4(pos, 1);
	v2f_normal = normalize((_MV * vec4(normal, 0)).xyz);
}
