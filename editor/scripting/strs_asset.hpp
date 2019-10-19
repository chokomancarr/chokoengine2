#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define STR(nm) #nm

const std::string EAssetTypeStrs[] = {
	STR(AnimClip),
	STR(AnimGraph),
	STR(Armature),
	STR(Font),
	STR(Material),
	STR(Mesh),
	STR(Shader),
	STR(VShader),
	STR(Texture),
	STR(SceneObject)
};

CE_END_ED_NAMESPACE