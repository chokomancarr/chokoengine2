{
  "name":"Standard (Specular)",
  "variables":{
    "diffuse":"Texture",
    "specular_smoothness":"Texture",
    "occlusion":"Texture"
  },

  "vertex":"
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 tangent;
layout(location=3) in vec2 texCoord;

uniform mat4 _MV;
uniform mat4 _P;
uniform mat4 _MVP;

out vec3 v2f_normal;
out vec3 v2f_tangent;
out vec2 v2f_uv;

void main(){
    gl_Position = _MVP*vec4(pos, 1);
    v2f_normal = normalize((_MV * vec4(normal, 0)).xyz);
    v2f_tangent = normalize((_MV * vec4(tangent, 0)).xyz);
    v2f_uv = texCoord;
}",

  "fragment":"
in vec3 v2f_normal;
in vec3 v2f_tangent;
in vec2 v2f_uv;

uniform int _object_id;

uniform sampler2D diffuse;
uniform sampler2D specular_smoothness;
uniform sampler2D occlusion;

layout (location=0) out vec4 outColor; //rgb?
layout (location=1) out vec4 outNormal; //xyz
layout (location=2) out vec4 outSpec; //metal rough occlu flags2
layout (location=3) out vec4 outEmi; //????

layout (location=4) out ivec4 _out_attrs; //????

void main() {
    outColor = texture(diffuse, v2f_uv);
    outNormal.xyz = normalize(v2f_normal);
    vec4 ss = texture(specular_smoothness, v2f_uv);
    outSpec.r = ss.r;
    outSpec.g = 1 - ss.a;
    outSpec.b = texture(occlusion, v2f_uv).r;
    outSpec.a = 0;
    outEmi = vec4(0, 0, 0, 0);
    
    _out_attrs.r = _object_id;
}"
}