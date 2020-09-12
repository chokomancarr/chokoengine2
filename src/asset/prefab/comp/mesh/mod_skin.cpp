#include "../../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

namespace PrefabMR {
	PrefabItem toPrefab(const MeshSkinModifier& mod) {
		PrefabItemGroup res = {};
		res.push_back(PrefabItem((Component)mod->rig().lock()));

		return PrefabItem(std::move(res));
	}

	void instMeshModifier(MeshSkinModifier& mod, const PrefabItemGroup& data) {
		for (auto& d : data) {
			if (d.name == "rig") {
				const auto cref = d.Get<Prefab_CompRef>();
				PrefabState::refresolvers.top().push_back([mod, cref]() {
					mod->rig((Rig)cref.Seek(PrefabState::activeBaseObjs.top()));
				});
			}
		}
	}
}

CE_END_NAMESPACE
