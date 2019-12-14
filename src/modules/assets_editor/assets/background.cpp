#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(Background) {
	const auto meta = LoadMeta(path);

	return Background::New(_basePath + path, 5, async);
}

CE_END_MOD_AE_NAMESPACE
