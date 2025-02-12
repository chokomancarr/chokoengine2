#include "../asset_loader.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

template <typename T>
T AssetLoader::Get(AssetType t, const std::string& s, bool async, bool forceReload) {
	auto& res = _cache[(int)t][s];
	if (forceReload || !res) {
		res = Load(t, s, async);
	}
	return (T)res;
}

CE_END_MOD_AA_NAMESPACE
