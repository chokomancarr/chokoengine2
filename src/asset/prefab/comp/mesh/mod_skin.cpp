#include "../../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

namespace PrefabMR {
	void instMeshModifier(MeshSkinModifier& mod, const PrefabItemGroup& data) {
		for (auto& d : data) {
			if (d.name == "rig") {
				mod->rig((Rig)d.Get<const Prefab_CompRef&>().Seek(
					{ PrefabState::activeBaseObjs.top()->children() }));
			}
		}
	}
}

CE_END_NAMESPACE
