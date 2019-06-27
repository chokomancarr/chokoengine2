#pragma once
namespace glsl {
	const char skyFrag[] = R"(
uniform mat4 _IP;
uniform vec2 screenSize;
uniform bool isOrtho;

uniform sampler2D inGBuf0;
uniform sampler2D inGBuf1;
uniform sampler2D inGBuf2;
uniform sampler2D inGBufD;

uniform sampler2D inSky;
uniform float skyStrength;

out vec4 fragCol;

float length2(vec3 v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

vec4 skyColAt(sampler2D sky, vec3 dir, float lod) {
    vec2 refla = normalize(-vec2(dir.x, dir.z));
    float cx = acos(refla.x)/(3.14159 * 2);
    cx = mix(1-cx, cx, sign(refla.y) * 0.5 + 0.5);
    float sy = asin(dir.y)/(3.14159);

    return textureLod(sky, vec2(cx, sy + 0.5), lod);
}

float fresnel(vec3 fwd, vec3 nrm) {
	return pow(1-dot(-fwd, nrm), 5);
}

void main () {
    vec2 uv = gl_FragCoord.xy / screenSize;
    vec4 diffuse = texture(inGBuf0, uv);
    vec3 normal = texture(inGBuf1, uv).xyz;

	vec4 gbuf2 = texture(inGBuf2, uv);
	float metallic = gbuf2.x;
	float rough = gbuf2.y;
	float occlu = gbuf2.z;
    float z = texture(inGBufD, uv).x;
	
	float nClip = 0.1;
    float fClip = 100.0;

    float zLinear;
    if (isOrtho) zLinear = z;
    else zLinear = (2 * nClip) / (fClip + nClip - (z * 2 - 1) * (fClip - nClip));
	

    vec4 dc = vec4(uv.x*2-1, uv.y*2-1, z*2-1, 1);
    vec4 wPos = _IP*dc;
    wPos /= wPos.w;
    vec4 wPos2 = _IP*vec4(uv.x*2-1, uv.y*2-1, -1, 1);
    wPos2 /= wPos2.w;
    vec3 fwd = normalize(wPos.xyz - wPos2.xyz);
	
	fragCol.rgb = skyColAt(inSky, fwd, 0).rgb * skyStrength;
	if (z < 1) {
		vec3 refl = normalize(reflect(fwd, normal));
		float fres = mix(fresnel(fwd, normal), 1, 0.1);
		vec3 diffCol = skyColAt(inSky, normal, 5).rgb * diffuse.rgb;
        vec3 reflCol = skyColAt(inSky, refl, rough * 5).rgb * mix(vec3(1, 1, 1), diffuse.rgb, metallic * (1 - fres));
		fragCol.rgb = mix(diffCol, reflCol, mix(fres, 1, metallic)) * skyStrength * occlu;
        //fragCol.rgb = skyColAt(inSky, normalize(normal), (1-fres) * 5).rgb * diffuse.rgb;
	}
    fragCol.a = 1;
    return;
}
)";
}