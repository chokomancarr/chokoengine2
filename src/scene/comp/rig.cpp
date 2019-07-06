#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Rig::_Rig() : CE_COMPDEF(Rig), _armature(nullptr), _boneObjs({}) {}

void _Rig::AddBones(const SceneObject& parent, const std::vector<Bone>& bones) {
    for (auto& b : bones) {
        auto obj = Scene::AddNewObject(parent);
        obj->name(b.name);
        AddBones(obj, b.children);
    }
}

void _Rig::armature(const Armature& arma) {
    if (!!_armature) {
        for (auto& b : _boneObjs) {
            if (b->components().size() > 0) {
                Debug::Warning("Rig::armature (set)", "Deleting bone \"" + b->name() + " with components attached!");
            }
            Scene::RemoveObject(b);
        }
        _boneObjs.clear();
    }
    _armature = arma;
    AddBones(object(), arma->bones());
}

CE_END_NAMESPACE