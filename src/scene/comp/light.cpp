#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _Light::Init() {
    return true;
}

void _Light::GenShadowMap() {
	if (!_shadow) return;
	if (_type == LightType::Point) {
		shadowBuffer_2D = nullptr;
		shadowBuffer_Cube = FrameBufferCube_New(_shadowResolution, { GL_RGBA });
	}
	else {
		shadowBuffer_Cube = nullptr;
		shadowBuffer_2D = FrameBuffer_New(_shadowResolution, _shadowResolution, { GL_RGBA });
	}
}

_Light::_Light(LightType t) : CE_COMPDEF(Light), _type(t), _falloff(LightFalloff::Squared),
        _strength(1), _distance(10), _radius(0), _angle(30), _color(Color::white()), shadowBuffer_Cube(nullptr), shadowBuffer_2D(nullptr), _shadow(false),
        _shadowDistance(50), _shadowStrength(1), _shadowBias(0.01f), _shadowResolution(512), _shadowOnly(false) {}

void _Light::type(const LightType& t) {
	if (t != _type) {
		_type = t;
		GenShadowMap();
	}
}

void _Light::shadow(const bool& s) {
	if (s != _shadow) {
		_shadow = s;
		GenShadowMap();
	}
}

void _Light::shadowResolution(const int& r) {
	if (r != _shadowResolution) {
		_shadowResolution = r;
		GenShadowMap();
	}
}

CE_END_NAMESPACE