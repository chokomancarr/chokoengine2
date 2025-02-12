#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Collider) {
	CE_PR_ADD(type);
	CE_PR_ADD(radius);
}

CE_PR_IMPL_COMP_INST(Collider) {
	auto c = o->AddComponent<Collider>();

	ApplyCollider(c);
}

CE_PR_IMPL_COMP_APP(Collider) {
	CE_PR_SET(type);
	CE_PR_SET(radius);
}

CE_END_NAMESPACE
