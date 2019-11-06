#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const SceneObject& o) : _rootObject(o) {}

SceneObject _Prefab::Instantiate() const {
    return _rootObject->Clone();
}

CE_END_NAMESPACE