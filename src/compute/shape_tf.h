#pragma once
namespace compute {
    const char* shape_tf = R"(
#version 330 core
layout(location=0) in vec3 pos;

uniform int num;
uniform samplerBuffer shps;
uniform samplerBuffer whts;

out vec3 outPos;

void main() {
    int gid = gl_VertexID;
    outPos = pos;
    for (int s = 0; s < num; s++) {
        outPos += texelFetch(shps, gid*num + s).xyz * texelFetch(whts, s).x;
    }
}
)";
}