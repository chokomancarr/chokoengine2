#pragma once
#include "inc/random.h"

namespace glsl {
	const std::string particlesys_tf = "#version 330 core\n"
+ glsl::inc::rand + R"(

#define PI 3.14159

layout(location=0) in vec4 inbuf0;
layout(location=1) in vec4 inbuf1;
layout(location=2) in vec4 inbuf2;

uniform int numParticles;
uniform int randSeed;
uniform float DT;

uniform int emissionShape;

uniform float radius;
uniform float angle;
uniform float length;

uniform vec2 pLifetime;
uniform vec2 pRotation0;
uniform vec2 pScale0;
uniform vec2 pSpeed0;
uniform vec2 pASpeed0;

uniform sampler1D pSpeedGrad;
uniform sampler1D pSizeGrad;

uniform vec3 pForce;
uniform vec3 pForceR;

out vec4 outbuf0;
out vec4 outbuf1;
out vec4 outbuf2;

float RND(inout float rnd) {
	float res = rnd;
	rnd = rand(rnd);
	return res;
} 

float sampleCR(vec2 v, inout float rnd) {
	return v.x + RND(rnd)*v.y;
}

void main () {
	float rnd = rand(rand(gl_VertexID + randSeed) + randSeed);

	float hp = inbuf0.x;
	float maxhp = inbuf0.y;
	float rot = inbuf0.z;
	vec3 pos = inbuf1.xyz;
	float scl = inbuf1.w;
	vec3 vel = inbuf2.xyz;
	float avel = inbuf2.w;


    // --------- particle generation ------------
	if (maxhp == 0) {

		hp = maxhp = max(sampleCR(pLifetime, rnd), 0.0001);
		rot = sampleCR(pRotation0, rnd);
		scl = sampleCR(pScale0, rnd);
		avel = sampleCR(pASpeed0, rnd);

		if (emissionShape == 0) { //cone
			float a = RND(rnd) * 2 * PI;
			float r = sqrt(RND(rnd));

			pos.x = cos(a);
			pos.y = sin(a);
			pos.z = 0;

			float th = r * angle;
			float sth = sin(th);

			vel = vec3(sth * pos.x, sth * pos.y, cos(th));
			pos *= radius * r;
		}
		else if (emissionShape == 1) { //sphere
			pos = vec3(0);
			float th = RND(rnd) * 2 * PI;
			float z = RND(rnd) * 2 - 1;
			float xy = sqrt(1 - z*z);
			vel = vec3(cos(th)*xy, sin(th)*xy, z);
		}

		vel *= sampleCR(pSpeed0, rnd);
	}

	hp -= DT;
	
    // --------- particle simulation ------------
	pos += vel * DT + 0.5 * pForce * DT*DT;
	vel += pForce * DT;
	rot += avel * DT;


	outbuf0.x = hp;
	outbuf0.y = maxhp;
	outbuf0.z = rot;
	outbuf1.xyz = pos;
	outbuf1.w = scl;
	outbuf2.xyz = vel;
	outbuf2.w = avel;
}
)";
}