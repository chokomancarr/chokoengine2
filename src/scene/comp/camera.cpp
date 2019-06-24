#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _Camera::Init() {
	return true;
}

_Camera::_Camera() : CE_COMPDEF(Camera), _target(nullptr),
		_orthographic(false), _fov(60), _orthoSize(1),
		_nearClip(0.1f), _farClip(100.f), _clearType(CameraClearType::ColorAndDepth),
		_clearDepth(1.f), _clearColor(0.f, 0.f) {

}

CE_END_NAMESPACE