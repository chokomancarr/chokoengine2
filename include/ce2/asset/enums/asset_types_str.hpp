/*
 * Generated by the preprocessor script
 * DO NOT EDIT
 */
#pragma once
#include "chokoengine.hpp"

namespace std { template <> struct hash<CE_NS AssetType> { size_t operator() (const CE_NS AssetType &t) const { return size_t(t); } }; }

CE_BEGIN_NAMESPACE

const std::unordered_map<AssetType, std::string> AssetTypeStr = {
	{ AssetType::AnimClip, "AnimClip" },
	{ AssetType::AnimGraph, "AnimGraph" },
	{ AssetType::Armature, "Armature" },
	{ AssetType::Background, "Background" },
	{ AssetType::Font, "Font" },
	{ AssetType::Material, "Material" },
	{ AssetType::Mesh, "Mesh" },
	{ AssetType::Prefab, "Prefab" },
	{ AssetType::Shader, "Shader" },
	{ AssetType::Texture, "Texture" },
	{ AssetType::_COUNT, "_COUNT" },
	{ AssetType::Unknown, "Unknown" }
};

CE_END_NAMESPACE
