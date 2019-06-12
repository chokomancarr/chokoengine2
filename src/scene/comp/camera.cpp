#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

bool _Camera::Init() {
	return true;
}

_Camera::_Camera() : _Component(ComponentType::Camera) {

}

END_CE_NAMESPACE