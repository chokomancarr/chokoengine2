#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class AssetType {
	AnimClip,
	AnimGraph,
	Armature,
	Font,
	Material,
	Mesh,
	Prefab,
	Shader,
	Texture,
	_COUNT,
	Unknown = 0xff
};

const std::array<std::string, (int)AssetType::_COUNT> AssetTypeStr = {
	"AnimClip",
	"AnimGraph",
	"Armature",
	"Font",
	"Material",
	"Mesh",
	"Prefab",
	"Shader",
	"Texture"
};

CE_END_NAMESPACE