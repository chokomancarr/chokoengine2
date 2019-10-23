#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Prefab::_Prefab(const std::vector<SceneObject>& o)
    : _objects(o) {}

void _Prefab::AddObject(const SceneObject& o) {
    std::function<bool(const std::vector<SceneObject>&)> inobj = 
            [&](const std::vector<SceneObject>& oo) {
        for (auto& s : oo) {
            if ((s == o) || inobj(s->children()))
                return true;
        }
        return false;
    };

    if (inobj(_objects)) return;
    _objects.push_back(o);
}

std::vector<SceneObject> _Prefab::Instantiate() {
    std::vector<SceneObject> res;
    for (const auto& o : _objects) {
        res.push_back(o->Clone());
    }
    return res;
}

CE_END_NAMESPACE