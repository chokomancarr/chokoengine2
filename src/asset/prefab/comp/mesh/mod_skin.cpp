#include "../../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

namespace PrefabMR {
	void instMeshModifier(MeshSkinModifier& mod, const PrefabItemGroup& data) {
		for (auto& d : data) {
			if (d.name == "rig") {
				const auto cref = d.Get<Prefab_CompRef>();
				PrefabState::refresolvers.top().push_back([mod, cref]() {
					mod->rig((Rig)cref.Seek(
						{ PrefabState::activeBaseObjs.top()->children() }));
				});
			}
		}
	}
}

CE_END_NAMESPACE
