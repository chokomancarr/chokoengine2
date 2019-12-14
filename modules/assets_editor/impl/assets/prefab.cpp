#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(Prefab) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(_basePath + path));
	return Prefab::New(data, [](const std::string& s) -> Prefab {
		return Get<Prefab>(AssetType::Prefab, s, true);
	});
}

CE_END_MOD_AE_NAMESPACE
