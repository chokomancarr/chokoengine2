#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _LightProbe::Init() {
	return true;
}

_LightProbe::_LightProbe() : CE_COMPDEF(LightProbe), _strength(1), _center({})
		, _area(50, 50, 50), _updateFrequency(LightProbeUpdateFrequency::OnStart)
		, _dirty(true), _resolution(0), _nearClip(0.1f), _farClip(20), _deferredBuffer(nullptr)
		, _fbo(nullptr), _clearType(CameraClearType::ColorAndDepth), _clearColor(0, 0), _clearDepth(1) {
	resolution(256);
}

#define CP(nm) _ ## nm(rhs._ ## nm)
_LightProbe::_LightProbe(const _LightProbe& rhs) : CE_COMPDEF(LightProbe), CP(strength),
		CP(center), CP(area), CP(updateFrequency), CP(dirty), _resolution(0), CP(nearClip),
		CP(farClip), _deferredBuffer(nullptr), _fbo(nullptr), CP(clearType), CP(clearColor),
		CP(clearDepth) {
	resolution(rhs._resolution);
}

void _LightProbe::resolution(const int& r) {
	if (_resolution != r) {
		_resolution = r;
		_fbo = FrameBufferCube_New(r, { GL_RGBA32F });
		_dirty = true;
	}
}

const CubeMap& _LightProbe::result() {
	return _fbo->map(0);
}

CE_END_NAMESPACE