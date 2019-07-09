#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Rig::_Rig() : CE_COMPDEF(Rig), _armature(nullptr), _boneObjs({}) {}

void _Rig::AddBones(const SceneObject& parent, const std::vector<Bone>& bones, const std::string& path) {
    for (auto& b : bones) {
        auto obj = Scene::AddNewObject(parent);
        obj->name(b.name);
        _boneObjs.push_back(std::pair<SceneObject, _Bone>(obj, _Bone(b, path)));
        AddBones(obj, b.children, path + b.name + "/");
    }
}

void _Rig::armature(const Armature& arma) {
    if (!!_armature) {
        for (auto& b : _boneObjs) {
            if (b.first->components().size() > 0) {
                Debug::Warning("Rig::armature (set)", "Deleting bone \"" + b.second.sig + " with components attached!");
            }
            Scene::RemoveObject(b.first);
        }
        _boneObjs.clear();
    }
    _armature = arma;
    AddBones(object(), arma->bones(), "");
}

CE_END_NAMESPACE