#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _LightProbe::Init() {
    return true;
}

_LightProbe::_LightProbe() : CE_COMPDEF(LightProbe), _strength(1), _center({})
        , _area(50, 50, 50), _updateFrequency(LightProbeUpdateFrequency::OnStart), _fbo(nullptr), _resolution(0) {
	resolution(256);
}

void _LightProbe::resolution(const int& r) {
    if (_resolution != r) {
        _resolution = r;
		_fbo = FrameBufferCube_New(r, { GL_RGBA });
    }
}

CE_END_NAMESPACE