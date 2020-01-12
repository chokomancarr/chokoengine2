#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EAssetManager {
public:
	static void Init();

    static Asset FromJson(const JsonObject&, bool async);

    static JsonObject ToJson(const Asset&, AssetType);

	class Ops;
};

CE_END_ED_NAMESPACE

#include "ops/asset_ops.hpp"