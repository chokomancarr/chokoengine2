#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

enum class EAssetType {
	AnimClip,
	AnimGraph,
    Armature,
    Font,
    Material,
    Mesh,
    Shader,
	VShader,
    Texture,
    SceneObject,
    _COUNT,
    Unknown = 0xff
};

CE_END_ED_NAMESPACE