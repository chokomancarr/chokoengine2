#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_AL_IMPL_J(Rig) {
    auto mr = obj->AddComponent<Rig>();

    for (auto& g : data.group) {
        const auto& k = g.key.string;
        const auto& v = g.value;
        if (k == "armature") {
            mr->armature(static_cast<Armature>(EAssetList::Get(AssetType::Armature, v.string)));
        }
    }

    return mr;
}

CE_END_ED_NAMESPACE