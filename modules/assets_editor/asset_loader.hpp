#pragma once
#include "chokoplayer.hpp"
#include "ae_defines.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

#define CE_MOD_AE_META_SIGNATURE "ChokoEngine Asset MetaData"
#define CE_MOD_AE_META_VERSION 20

#define CE_MOD_AE_DEF(nm) static nm Load ## nm(const std::string&, bool);
#define CE_MOD_AE_IMPL(nm) nm AssetLoader::Load ## nm(const std::string& path, bool async)

class AssetLoader {
	static CE_AE_EXPORT std::string _basePath;
	static CE_AE_EXPORT std::array<std::unordered_map<std::string, Asset>, (int)AssetType::_COUNT> _cache;

public:
	static void Init(const std::string& basePath);

	static JsonObject LoadMeta(const std::string&);

	template <typename T>
	static T Get(AssetType, const std::string&, bool, bool forceReload = false);

	static Asset Load(AssetType, const std::string&, bool);

	CE_MOD_AE_DEF(AnimClip);
	CE_MOD_AE_DEF(Armature);
	CE_MOD_AE_DEF(Background);
	CE_MOD_AE_DEF(Material);
	CE_MOD_AE_DEF(Mesh);
	CE_MOD_AE_DEF(Prefab);
	CE_MOD_AE_DEF(Shader);
	CE_MOD_AE_DEF(Texture);
};

CE_END_MOD_AE_NAMESPACE

#include "impl/asset_loader.inl"
