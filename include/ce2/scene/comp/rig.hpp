#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Rig : public _Component { CE_COMPONENT_COMMON
    Armature _armature;

	struct boneObjSt {
		pSceneObject obj;
		ArmatureBoneG bone;

		boneObjSt(const pSceneObject& o, const ArmatureBoneG& b)
			: obj(o), bone(b) {}
	};
    std::vector<boneObjSt> _boneObjs;
	std::vector<Mat4x4> _matrices;

    void AddBones(const SceneObject& parent, const std::vector<ArmatureBone>& bones, const ArmatureBoneG* pbone, int pi, const Mat4x4& ib);
public:
	_Rig();

	_Rig(const _Rig&) = default;

	/* The armature configuration
	*/
    CE_GET_SET_MEMBER_F(armature);

    /* The scene objects corresponding to each bone
     */
    CE_GET_MEMBER(boneObjs);

	CE_GET_MEMBER(matrices);

	int BoneIndex(const std::string& sig);

	void OnLateUpdate() override;
};

CE_END_NAMESPACE

#include "mesh/modifier_skin.hpp"