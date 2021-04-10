#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

namespace PrefabMR {
	PrefabItem toPrefab(const MeshSkinModifier& mod);

	void instMeshModifier(const MeshShapeModifier& mod, const PrefabItemGroup& data);
	void instMeshModifier(const MeshSkinModifier& mod, const PrefabItemGroup& data);
	void instMeshModifier(const MeshClothModifier& mod, const PrefabItemGroup& data);
}

CE_PR_IMPL_COMP(MeshRenderer) {
	CE_PR_ADD(mesh);

	/*
	const auto& mods = c->modifiers();
	if (!!mods.size()) {
		PrefabItemGroup modres;
		for (auto& m : mods) {
			if (m->type == MeshModifierType::Skin) {
				modres.push_back(PrefabMR::toPrefab((MeshSkinModifier)m));
			}
			else {
				CE_NOT_IMPLEMENTED
			}
		}
		items.push_back(std::make_pair("modifiers", std::move(modres)));
	}
	*/

	const auto& mats = c->materials();
	if (!!mats.size()) {
		PrefabItem matres(PrefabItem::Type::Asset);
		matres.is_array = true;
		for (auto& m : mats) {
			matres.value.group.push_back((Asset)m);
		}
		items.push_back(std::make_pair("materials", std::move(matres)));
	}
}

CE_PR_IMPL_COMP_INST(MeshRenderer) {
	auto c = o->AddComponent<MeshRenderer>();
	
	ApplyMeshRenderer(c);
}

CE_PR_IMPL_COMP_APP(MeshRenderer) {
	CE_PR_SET_A(mesh, Mesh);
	const auto& mods = _CE_PR_GET<const PrefabItemGroup&>(this, "modifiers", {});
	for (auto& m : mods) {
		if (m.name == "skin") {
			PrefabMR::instMeshModifier(c->AddModifier<MeshSkinModifier>(), m.value.group);
		}
	}
	static const PrefabItemGroup grp = {};
	const auto& mats = _CE_PR_GET<const PrefabItemGroup&>(this, "materials", grp);
	if (&mats == &grp) {
		std::vector<Material> _mats = c->materials();
		const auto n = mats.size();
		_mats.resize(std::max(n, _mats.size()));
		for (int a = 0; a < n; a++) {
			if (!mats[a].is_default)
				_mats[a] = ((Material)mats[a].Get<Asset>());
		}
		c->materials(_mats);
	}
}

CE_END_NAMESPACE
