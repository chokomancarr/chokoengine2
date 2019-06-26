in vec3 v2f_normal;
in vec3 v2f_tangent;
in vec2 v2f_uv;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D occluTex;
uniform vec4 col;
uniform float metallic;
uniform float roughness;
uniform float normalness;

layout (location=0) out vec4 outColor; //rgb?
layout (location=1) out vec4 outNormal; //xyz
layout (location=2) out vec4 outSpec; //flags1 rough occlu flags2
layout (location=3) out vec4 outEmi; //????

void main() {
    outColor = col * texture(diffuseTex, v2f_uv);
    vec3 nrm = normalize(v2f_normal);
    vec3 tgt = normalize(v2f_tangent);
    vec3 btgt = cross(nrm, tgt);
    vec3 nrmCol = texture(normalTex, v2f_uv).xyz;
    outNormal.xyz = normalize(mix(nrm.xyz, tgt * (nrmCol.x * 2 - 1) + btgt * (nrmCol.y * 2 - 1) + nrm.xyz * nrmCol.z, normalness));
    int flags1 = (metallic > 0.5) ? 1 : 0;
    outSpec.r = (flags1 + 0.05) * 0.125;
    outSpec.g = roughness;
    outSpec.b = texture(occluTex, v2f_uv).r;
    outSpec.a = 0;
    outEmi = vec4(0, 0, 0, 0);
}
