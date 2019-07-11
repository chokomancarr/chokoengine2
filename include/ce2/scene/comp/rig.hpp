#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Rig : public _Component { CE_COMPONENT_COMMON
    Armature _armature;

	struct boneObjSt {
		pSceneObject obj;
		_Bone bone;

		boneObjSt(const pSceneObject& o, const _Bone& b)
			: obj(o), bone(b) {}
	};
    std::vector<boneObjSt> _boneObjs;
	std::vector<Mat4x4> _matrices;

    void AddBones(const SceneObject& parent, const std::vector<Bone>& bones, const _Bone* pbone, const std::string& path);

public:
	_Rig();

	/* The armature configuration
	*/
    CE_GET_SET_MEMBER_F(armature);

    /* The scene objects corresponding to each bone
     */
    CE_GET_MEMBER(boneObjs);

	CE_GET_MEMBER(matrices);

	int BoneIndex(const std::string& sig);

	void OnUpdate() override;
};

CE_END_NAMESPACE

#include "mesh/modifier_skin.hpp"