#pragma once
namespace compute {
    const char* skin_tf_mat = R"(
#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 tangent;

uniform mat4 Md; //model space to rig space
uniform mat4 iMd;

uniform isamplerBuffer mids;
uniform samplerBuffer mws;
uniform samplerBuffer mats;

out vec3 outPos;
out vec3 outNrm;
out vec3 outTgt;

mat4 buf2mat (int id) {
    id *= 4;
    mat4 res;
    res[0] = texelFetch(mats, id);
    res[1] = texelFetch(mats, id+1);
    res[2] = texelFetch(mats, id+2);
    res[3] = texelFetch(mats, id+3);
    return res;
}

void main() {
    int gid = gl_VertexID;
    ivec4 ids = texelFetch(mids, gid);
    vec4 ws = texelFetch(mws, gid);
    mat4 m = buf2mat(ids[0]);//*ws[0]
//        + buf2mat(ids[1])*ws[1]
//        + buf2mat(ids[2])*ws[2]
//        + buf2mat(ids[3])*ws[3];
    vec4 p = vec4(pos, 1);
    vec4 n = vec4(normal, 0);
    vec4 t = vec4(tangent, 0);
    p.w = 1;
    //p = iMd * m * Md * p;
    p = m * p;
    outPos = p.xyz / p.w;
    outNrm = normal;//m * n;
    outTgt = tangent;//t * t;
}
)";
}