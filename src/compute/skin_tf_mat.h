#pragma once
namespace compute {
    const char* skin_tf_mat = R"(
#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 tangent;

uniform ivec4 params;

uniform samplerBuffer dats;
uniform samplerBuffer mats;
//uniform samplerBuffer shps;
//uniform samplerBuffer shpWs;

out vec3 outPos;
out vec3 outNrm;
out vec3 outTgt;

mat4 buf2mat (samplerBuffer buf, int id) {
    id *= 4;
    mat4 res;
    res[0] = texelFetch(buf, id);
    res[1] = texelFetch(buf, id+1);
    res[2] = texelFetch(buf, id+2);
    res[3] = texelFetch(buf, id+3);
    return res;
}

void main() {
    int gid = gl_VertexID;
    vec4 dt_mats = texelFetch(dats, gid*2);
    vec4 dt_ws = texelFetch(dats, gid*2+1);
    //mat4 m = buf2mat(mats, int(dt_mats[0]))*dt_ws[0]
    //    + buf2mat(mats, int(dt_mats[1]))*dt_ws[1]
    //    + buf2mat(mats, int(dt_mats[2]))*dt_ws[2]
    //    + buf2mat(mats, int(dt_mats[3]))*dt_ws[3];
    vec4 p = vec4(pos, 1);
    vec4 n = vec4(normal, 0);
    vec4 t = vec4(tangent, 0);
//    for (int s = 0; s < params.y; s++) {
//        p += texelFetch(shps, s*params.x + gid) * texelFetch(shpWs, s).r;
//    }
    p.w = 1;
    //p = m * p;
    outPos = pos;//p / p.w;
    outNrm = normal;//m * n;
    outTgt = tangent;//t * t;
}
)";
}