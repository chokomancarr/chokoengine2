#pragma once
#include "chokoplayer.hpp"
#include "detail/aa_defines.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

#define CE_MOD_AA_DEF(nm) static nm Load ## nm(const std::string&, bool);
#define CE_MOD_AA_IMPL(nm) nm AssetLoader::Load ## nm(const std::string& path, bool async)

class AssetLoader {
	static std::string _basePath;
	static std::array<std::unordered_map<std::string, Asset>, (int)AssetType::_COUNT> _cache;

public:
	static void Init(const std::string& basePath);

	template <typename T>
	static T Get(AssetType, const std::string&, bool, bool forceReload = false);

	static Asset Load(AssetType, const std::string&, bool);

	CE_MOD_AA_DEF(AnimClip);
	CE_MOD_AA_DEF(Armature);
	CE_MOD_AA_DEF(Background);
	CE_MOD_AA_DEF(Material);
	CE_MOD_AA_DEF(Mesh);
	CE_MOD_AA_DEF(Prefab);
	CE_MOD_AA_DEF(Shader);
	CE_MOD_AA_DEF(Texture);
};

CE_END_MOD_AA_NAMESPACE

#include "detail/asset_loader.inl"
