#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Rig::_Rig() : CE_COMPDEF(Rig), _armature(nullptr), _boneObjs({}) {}

void _Rig::AddBones(const SceneObject& parent, const std::vector<Bone>& bones, const _Bone* pbone, const std::string& path) {
    for (auto& b : bones) {
        auto rot = Quat::LookAt(b.length, b.front);
        if (std::isnan(rot.x) || std::isnan(rot.y) || std::isnan(rot.z) || std::isnan(rot.w)) {
            rot = Quat::identity();
        }
        auto obj = Scene::AddNewObject(parent);

        obj->name(b.name);
        auto tr = obj->transform();
        if (pbone) {
            tr->localPosition(b.base + Vec3(0, 0, 1) * pbone->length);
        }
        else {
            tr->localPosition(b.base);
        }
        tr->localRotation(rot);

        const auto _bn = _Bone(b, path);
        _boneObjs.push_back(std::pair<SceneObject, _Bone>(obj, _bn));
        AddBones(obj, b.children, &_bn, path + b.name + "/");
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
    AddBones(object(), arma->bones(), nullptr, "");
}

CE_END_NAMESPACE