#pragma once
#include "glsl/minVert.h"

namespace glsl {
	const auto voxelAOVert = minVert;
	const char voxelAOFrag[] = R"(
#version 330 core

uniform vec2 screenSize;

uniform mat4 _IP;
uniform mat4 _VP;

uniform sampler2D nrmTex;
uniform sampler2D dphTex;

uniform isampler3D voxTex;

uniform float radius;

out vec4 fragCol;


uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

float rand( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float rand( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float rand( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }

const float pi2 = 3.14159 / 2;

void main() {
	vec2 uv = gl_FragCoord.xy / screenSize;
	float z = texture(dphTex, uv).x;
	vec4 wPos = _IP * vec4(uv.x*2-1, uv.y*2-1, z*2-1, 1);
	wPos /= wPos.w;
	vec3 nrm = texture(nrmTex, uv).xyz;
	vec3 tan = normalize(cross(nrm, vec3(0, 1, 0)));
	if (length(tan) == 0) tan = normalize(cross(nrm, vec3(0, 0, 1)));
	vec3 bitan = normalize(cross(nrm, tan));
	float rnd = rand(wPos.xyz);
	float str = 0;
	for (int a = 0; a < 20; a++) {
		float ph = rand(rnd);
		ph *= ph;
		float th = rand(ph) * cos(ph * pi2);
		vec3 dir = nrm * sin(ph * pi2) + tan * cos(th * pi2) + cotan * sin(th * pi2);
		rnd = rand(th);
		dir *= rnd * radius;
		vec4 vuv = _VP * vec4(wPos + dir, 1);
		str += texture(voxTex, (vuv.xyz / vuv.w) * 0.5 + 0.5).r;
	}
	str = 1 - str / 20;
	fragCol = vec4(str, str, str, 1);
}
)";
}