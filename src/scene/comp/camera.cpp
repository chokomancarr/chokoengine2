#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _Camera::Init() {
	return true;
}

_Camera::_Camera() : _Component(ComponentType::Camera) {

}

CE_END_NAMESPACE