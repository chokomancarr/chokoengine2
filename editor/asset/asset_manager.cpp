#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EAssetManager::Init() {
	Ops::Reg();
}

Asset EAssetManager::FromJson(const JsonObject& json, bool async) {
    const auto& g = json.group[0];
    const auto& vls = g.value.string;
    if (vls == "none") return nullptr;
    for (auto& a : AssetTypeStr) {
        if (a.second == g.key.string) {
            return EAssetList::Get(a.first, vls, async);
        }
    }
}

JsonObject EAssetManager::ToJson(const Asset& a, AssetType t) {
    JsonObject vl(!a ? "none" : a->assetSignature());
    return JsonObject({ JsonPair(JsonObject(AssetTypeStr.at(t)), vl) });
};

CE_END_ED_NAMESPACE