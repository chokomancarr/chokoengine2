#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

template <typename T>
static T AssetLoader::Get(AssetType t, const std::string& s, bool forceReload) {
	auto& res = _cache[(int)t][s];
	if (forceReload || !res) {
		res = Load(t, s, true);
	}
	return (T)res;
}

CE_END_MOD_AE_NAMESPACE
