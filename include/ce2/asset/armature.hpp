#pragma once
#include "chokoengine.hpp"
#include "armature_bone.hpp"

CE_BEGIN_NAMESPACE

class _Armature : public _Asset { CE_OBJECT_COMMON
public:

private:
    std::vector<Bone> _bones;

public:
    _Armature();

    CE_GET_SET_MEMBER(bones);
};

CE_END_NAMESPACE