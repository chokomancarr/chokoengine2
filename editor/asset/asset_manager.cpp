#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

Asset EAssetManager::FromJson(const JsonObject& json, bool async) {
    const auto& g = json.group[0];
    const auto& vls = g.value.string;
    if (vls == "none") return nullptr;
    for (int a = 0; a < (int)AssetType::_COUNT; a++) {
        if (AssetTypeStr[a] == g.key.string) {
            return EAssetList::Get((AssetType)a, vls, async);
        }
    }
}

JsonObject EAssetManager::ToJson(const Asset& a, AssetType t) {
    JsonObject vl(!a ? "none" : a->assetSignature());
    return JsonObject({ JsonPair(JsonObject(AssetTypeStr[(int)t]), vl) });
};

CE_END_ED_NAMESPACE