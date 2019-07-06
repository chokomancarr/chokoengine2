#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Rig : public _Component { CE_COMPONENT_COMMON
    Armature _armature;

    std::vector<SceneObject> _boneObjs;

    void AddBones(const SceneObject& parent, const std::vector<Bone>& bones);

public:
	_Rig();

	/* The armature configuration
	*/
    CE_GET_SET_MEMBER_F(armature);
};

CE_END_NAMESPACE

#include "mesh/modifier_skin.hpp"