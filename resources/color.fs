in vec3 v2f_normal;
in vec2 v2f_uv;

uniform sampler2D diffuseTex;
uniform sampler2D occluTex;
uniform vec4 col;
uniform float metallic;
uniform float roughness;

layout (location=0) out vec4 outColor; //rgb?
layout (location=1) out vec4 outNormal; //xyz
layout (location=2) out vec4 outSpec; //flags1 rough occlu flags2
layout (location=3) out vec4 outEmi; //????

void main() {
    outColor = col * texture(diffuseTex, v2f_uv);
    vec3 nrm = normalize(v2f_normal);
    outNormal.xyz = nrm.xyz;
    int flags1 = (metallic > 0.5) ? 1 : 0;
    outSpec.r = (flags1 + 0.05) * 0.125;
    outSpec.g = roughness;
    outSpec.b = texture(occluTex, v2f_uv).r;
    outSpec.a = 0;
    outEmi = vec4(0, 0, 0, 0);
}
