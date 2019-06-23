#pragma once
namespace glsl {
	const char skyFrag[] = R"(
uniform mat4 _IP;
uniform vec2 screenSize;
uniform bool isOrtho;

uniform sampler2D inColor;
uniform sampler2D inNormal;
uniform sampler2D inDepth;

uniform sampler2D inSky;
uniform sampler2D inSkyE;
uniform float skyStrength;

out vec4 fragCol;

float length2(vec3 v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

vec4 skyColAt(sampler2D sky, vec3 dir) {
    vec2 refla = normalize(-vec2(dir.x, dir.z));
    float cx = acos(refla.x)/(3.14159 * 2);
    cx = mix(1-cx, cx, sign(refla.y) * 0.5 + 0.5);
    float sy = asin(dir.y)/(3.14159);

    return texture(sky, vec2(cx, sy + 0.5));
}

void main () {
    vec2 uv = gl_FragCoord.xy / screenSize;
    //vec4 dCol = texture(inColor, uv);
    //vec4 nCol = texture(inNormal, uv);
    //float z = texture(inDepth, uv).x;
    
	/*
	float nClip = 0.1;
    float fClip = 100.0;

    float zLinear;
    if (isOrtho) zLinear = z;
    else zLinear = (2 * nClip) / (fClip + nClip - (z * 2 - 1) * (fClip - nClip));
	*/

    vec4 dc = vec4(uv.x*2-1, uv.y*2-1, 1, 1);
    vec4 wPos = _IP*dc;
    wPos /= wPos.w;
    vec4 wPos2 = _IP*vec4(uv.x*2-1, uv.y*2-1, -1, 1);
    wPos2 /= wPos2.w;
    vec3 fwd = normalize(wPos.xyz - wPos2.xyz);
	
	fragCol.rgb = skyColAt(inSky, fwd).rgb * skyStrength;
    fragCol.a = 1;
    return;
}
)";
}