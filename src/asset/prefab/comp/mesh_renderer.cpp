#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(MeshRenderer) {
	CE_PR_ADD(mesh);
}

CE_PR_IMPL_COMP_INST(MeshRenderer) {
	auto c = o->AddComponent<MeshRenderer>();
	CE_PR_SET_A(mesh, Mesh);
}

CE_END_NAMESPACE
