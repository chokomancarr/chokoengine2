#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Rig::_Rig() : CE_COMPDEF(Rig), _armature(nullptr), _boneObjs({}) {}

void _Rig::AddBones(const SceneObject& parent, const std::vector<Bone>& bones, const _Bone* pbone, int pi, const Mat4x4& ib) {
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

        const auto _bn = _Bone(b, (pi == -1) ? "/" : (_boneObjs[pi].bone.sig + "/"), pi, ib * tr->worldMatrix());
        _boneObjs.push_back(boneObjSt(pSceneObject(obj), _bn));
        AddBones(obj, b.children, &_bn, (int)(_boneObjs.size()-1), ib);
    }
}

void _Rig::armature(const Armature& arma) {
    if (!!_armature) {
        for (auto& b : _boneObjs) {
            if (b.obj->components().size() > 0) {
                Debug::Warning("Rig::armature (set)", "Deleting bone \"" + b.bone.sig + " with components attached!");
            }
            Scene::RemoveObject(b.obj.lock());
        }
        _boneObjs.clear();
    }
    _armature = arma;
	const auto ib = object()->transform()->worldMatrix().inverse();
    AddBones(object(), arma->bones(), nullptr, -1, ib);
	_matrices.resize(_boneObjs.size());
}

int _Rig::BoneIndex(const std::string& nm) {
	auto it = std::find_if(_boneObjs.begin(), _boneObjs.end(), [&](const boneObjSt& o) {
		return o.obj->name() == nm;
	});
	if (it == _boneObjs.end()) {
		//Debug::Warning("Rig::BoneIndex", "No bone found with signature \"" + nm + "\"!");
		return -1;
	}
	return (int)(it - _boneObjs.begin());
}

void _Rig::OnUpdate() {
	const auto& sz = _boneObjs.size();
	for (size_t a = 0; a < sz; a++) {
        auto& bo = _boneObjs[a];
        if (bo.bone.parentId == -1) bo.bone.currMat = bo.obj->transform()->localMatrix();
        else bo.bone.currMat = _boneObjs[bo.bone.parentId].bone.currMat * bo.obj->transform()->localMatrix();
		_matrices[a] = bo.bone.currMat * bo.bone.restMatI;
	}
}

CE_END_NAMESPACE