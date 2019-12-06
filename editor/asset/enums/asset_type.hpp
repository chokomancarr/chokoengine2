#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T>
struct EAssetTypeOf;

#define _EAssetTypeFromClass(nm) \
template <>\
struct EAssetTypeOf<nm> {\
	static const auto value = AssetType::nm;\
};

_EAssetTypeFromClass(AnimClip)
_EAssetTypeFromClass(AnimGraph)
_EAssetTypeFromClass(Armature)
_EAssetTypeFromClass(Font)
_EAssetTypeFromClass(Material)
_EAssetTypeFromClass(Mesh)
_EAssetTypeFromClass(Prefab)
_EAssetTypeFromClass(Shader)
_EAssetTypeFromClass(Texture)

#undef _EAssetTypeFromClass

CE_END_ED_NAMESPACE