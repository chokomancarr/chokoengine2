#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define STR(nm) #nm

const std::string ComponentTypeStrs[] = {
	STR(Animator),
	STR(Camera),
	STR(Light),
	STR(LightProbe),
	STR(MeshRenderer),
	STR(Rig),
	STR(Script)
};

CE_END_ED_NAMESPACE