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
    Prefab,
    Shader,
    Texture,
	VShader,
    _COUNT,
    Unknown = 0xff
};

const std::array<std::string, (int)EAssetType::_COUNT> EAssetTypeStr = {
	"AnimClip",
	"AnimGraph",
	"Armature",
	"Font",
	"Material",
	"Mesh",
	"Prefab",
	"Shader",
	"Texture",
	"VShader"
};

template <typename T>
struct EAssetTypeOf;

#define _EAssetTypeFromClass(nm) \
template <>\
struct EAssetTypeOf<nm> {\
	static const auto value = EAssetType::nm;\
};

_EAssetTypeFromClass(AnimClip)
_EAssetTypeFromClass(AnimGraph)
_EAssetTypeFromClass(Armature)
_EAssetTypeFromClass(Font)
_EAssetTypeFromClass(Material)
_EAssetTypeFromClass(Mesh)
_EAssetTypeFromClass(Prefab)
_EAssetTypeFromClass(Shader)
_EAssetTypeFromClass(VShader)
_EAssetTypeFromClass(Texture)

#undef _EAssetTypeFromClass

CE_END_ED_NAMESPACE