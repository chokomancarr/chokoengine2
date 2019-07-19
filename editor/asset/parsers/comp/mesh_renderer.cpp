#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_AL_IMPL_J(MeshRenderer) {
    auto mr = obj->AddComponent<MeshRenderer>();

    for (auto& g : data.group) {
        const auto& k = g.key.string;
        const auto& v = g.value;
        if (k == "mesh") {
            mr->mesh(static_cast<Mesh>(EAssetList::Get(EAssetType::Mesh, v.string)));
        }
		else if (k == "modifiers") {
			for (auto& g : g.value.group) {
				if (g.key.string == "shape") {
					mr->AddModifier<MeshShapeModifier>();
				}
				else if (g.key.string == "skin") {
					mr->AddModifier<MeshSkinModifier>();
				}
			}
		}
        else if (k == "materials") {
            std::vector<Material> mats;
            for (auto& m : v.list) {
                if (m.string != "") {
                    mats.push_back(static_cast<Material>(EAssetList::Get(EAssetType::Material, m.string)));
                }
            }
            mr->materials(mats);
        }
    }

    return mr;
}

CE_END_ED_NAMESPACE