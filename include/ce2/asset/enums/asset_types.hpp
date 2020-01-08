#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class AssetType {
	AnimClip,
	AnimGraph,
	Armature,
	Background,
	Font,
	Material,
	Mesh,
	Prefab,
	Shader,
	Texture,
	_COUNT,
	Unknown = 0xff
};

CE_END_NAMESPACE

#include "asset_types_str.hpp"
