#pragma once
namespace glsl {
	const char transOverlayFrag[] = R"(
uniform mat4 _P;
uniform mat4 _IP;
uniform vec2 screenSize;
uniform vec3 camPos;

uniform sampler2D trTex;
uniform sampler2D trNrm;
uniform sampler2D trPrm;
uniform sampler2D trDep;
uniform sampler2D opTex;
uniform sampler2D opDep;

out vec4 fragCol;

vec3 cam2world(vec3 v) {
    vec4 res = _IP * vec4(v, 1);
    return res.xyz / res.w;
}
vec3 world2cam(vec3 v) {
    vec4 res = _P * vec4(v, 1);
    return res.xyz / res.w;
}

bool refract2(vec3 i, vec3 n, float ior, out vec3 res) {
    ior = 1 / ior;
    float ni = dot(n, i);
    float k = 1 - ior*ior*(1 - ni*ni);
    if (k < 0) {
        res = vec3(0, 0, 0);
        return false;
    }
    res = ior*i - (ior*ni - sqrt(k))*n;
    return true;
}

void main () {
    vec2 uv = gl_FragCoord.xy / screenSize;
    vec4 tc = texture(trTex, uv);
    float td = texture(trDep, uv).r;

    vec3 tpos = cam2world(vec3(uv.x*2-1, uv.y*2-1, td*2-1));
    vec3 cpos = cam2world(vec3(uv.x*2-1, uv.y*2-1, -1));
    vec3 fwd = normalize(tpos - cpos);

    float ior = texture(trPrm, uv).r * 3;
    vec3 nrm = texture(trNrm, uv).rgb;
    float od = texture(opDep, uv).r;
    if (td < od) {
        //screen-space refraction
        vec3 opos = cam2world(vec3(uv.x*2-1, uv.y*2-1, od*2-1));
        float lto = length(opos - tpos);
        vec3 refr = refract(fwd, nrm, 1.0 / ior);
        vec3 oposr = tpos + refr * lto;
        vec3 sp = world2cam(oposr) * 0.5 + 0.5;
        vec4 oc = texture(opTex, sp.xy);
        fragCol = mix(oc, tc, min(max(tc.a, 0), 1));
    }
    else {
        vec4 oc = texture(opTex, uv);
        fragCol = oc;
    }
	gl_FragDepth = od;
}
)";
}