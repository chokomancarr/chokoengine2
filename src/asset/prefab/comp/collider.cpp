#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Collider) {

}

CE_PR_IMPL_COMP_INST(Collider) {
	auto c = o->AddComponent<Collider>();

}

CE_END_NAMESPACE
