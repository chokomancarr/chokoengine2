#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(MeshRenderer) {
	CE_PR_ADD(mesh);
}

CE_PR_IMPL_COMP_INST(MeshRenderer) {
	auto c = o->AddComponent<MeshRenderer>();
	CE_PR_SET_A(mesh, Mesh);
	const auto& mats = _CE_PR_GET<const PrefabItemGroup&>(this, "materials", {});
	std::vector<Material> _mats;
	for (auto& m : mats){
		_mats.push_back((Material)m.Get<Asset>());
	}
	c->materials(_mats);
}

CE_END_NAMESPACE
