#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

void _SceneObject::RemoveComponent(const Component& c) {
    auto cc = std::find(_components.begin(), _components.end(), c);
    if (cc == _components.end()) {
        Debug::Warning("SceneObject", "Cannot remove component: component is not attached!");
    }
    std::swap(*cc, _components.back());
    _components.pop_back();
}

CE_END_NAMESPACE