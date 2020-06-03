#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Component::_Component(ComponentType t, const std::string& nm)
		: _Object(nm), _callbackMask(~uint32_t(0)), componentType(t) {}

SceneObject _Component::object() {
	return _object.lock();
}

CE_END_NAMESPACE