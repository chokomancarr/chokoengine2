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
    Texture,
    SceneObject,
    _COUNT,
    Unknown
};

CE_END_ED_NAMESPACE