#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EAssetManager {

public:
    static Asset FromJson(const JsonObject&, bool async);

    static JsonObject ToJson(const Asset&, AssetType);
};

CE_END_ED_NAMESPACE