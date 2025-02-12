#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Rig::_Rig() : CE_COMPDEF(Rig), _armature(nullptr), _boneObjs({}) {}

void _Rig::AddBones(const SceneObject& parent, const std::vector<ArmatureBone>& bones, const ArmatureBoneG* pbone, int pi, const Mat4x4& ib) {
    for (auto& b : bones) {
        auto rot = Quat::LookAt(b.length, -b.front);
        if (std::isnan(rot.x) || std::isnan(rot.y) || std::isnan(rot.z) || std::isnan(rot.w)) {
            rot = Quat::identity();
        }
		auto obj = parent->AddNewChild();

        obj->name(b.name);
        auto tr = obj->transform();
		tr->worldPosition(b.base + object()->transform()->worldPosition());
		tr->worldRotation(rot);

        const auto _bn = ArmatureBoneG(b, (pi == -1) ? "" : (_boneObjs[pi].bone.sig + "/"), pi, ib * tr->worldMatrix());
        _boneObjs.push_back(boneObjSt(pSceneObject(obj), _bn));
        AddBones(obj, b.children, &_bn, (int)(_boneObjs.size()-1), ib);
    }
}

void _Rig::armature(const Armature& arma) {
    if (!!_armature) {
        for (auto& b : _boneObjs) {
			auto obj = b.obj.lock();
            if (obj->components().size() > 0) {
                Debug::Warning("Rig::armature (set)", "Deleting bone \"" + b.bone.sig + " with components attached!");
            }
			obj->scene()->RemoveObject(obj);
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

void _Rig::OnLateUpdate() {
	auto anim = _object->parent()->GetComponent<Animator>();

	const auto ib = object()->transform()->worldMatrix().inverse();
	const auto& sz = _boneObjs.size();
	for (size_t a = 0; a < sz; a++) {
		auto& bo = _boneObjs[a];

		if (!!anim) {
			auto tr = bo.obj->transform();
			auto vq = anim->Get(bo.bone.sig + "@T");
			if (vq.valid) tr->localPosition(*(Vec3*)&vq.v);
			vq = anim->Get(bo.bone.sig + "@R");
			if (vq.valid) tr->localRotation(vq.q);
			vq = anim->Get(bo.bone.sig + "@S");
			if (vq.valid) tr->localScale(*(Vec3*)&vq.v);
		}

		bo.bone.currMat = ib * bo.obj->transform()->worldMatrix();
		_matrices[a] = bo.bone.currMat * bo.bone.restMatI;
	}
}

CE_END_NAMESPACE