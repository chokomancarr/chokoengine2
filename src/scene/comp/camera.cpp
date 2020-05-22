#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _Camera::Init() {
	return true;
}

_Camera::_Camera() : CE_COMPDEF(Camera), _target(nullptr),
_blitTargets(), _orthographic(false), _fov(60), _orthoSize(10),
_nearClip(0.1f), _farClip(100.f), _clearType(CameraClearType::ColorAndDepth),
_clearDepth(1.f), _clearColor(0.f, 0.f), _lastViewProjectionMatrix(Mat4x4::Identity()), _effects({}) {}

#define CP(nm) _ ## nm(r._ ## nm)
_Camera::_Camera(const _Camera& r)
		: CE_COMPDEF(Camera), _deferredBuffer(nullptr), CP(target),
		_blitTargets({}), CP(orthographic), CP(fov), CP(orthoSize),
		CP(nearClip), CP(farClip), CP(clearType), CP(clearDepth), CP(clearColor),
		_lastViewProjectionMatrix(Mat4x4::Identity()), _effects({}) {
	//copy effects here
}

void _Camera::AddEffect(const CameraEffect& e) {
	_effects.push_back(e);
}

CE_END_NAMESPACE