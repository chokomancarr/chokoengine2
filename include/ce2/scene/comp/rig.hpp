#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Rig : public _Component { CE_COMPONENT_COMMON
    Armature _armature;

    std::vector<std::pair<SceneObject, _Bone>> _boneObjs;

    void AddBones(const SceneObject& parent, const std::vector<Bone>& bones, const std::string& path);

public:
	_Rig();

	/* The armature configuration
	*/
    CE_GET_SET_MEMBER_F(armature);

    /* The scene objects corresponding to each bone
     */
    CE_GET_MEMBER(boneObjs);
};

CE_END_NAMESPACE

#include "mesh/modifier_skin.hpp"