#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Rigidbody) {

}

CE_PR_IMPL_COMP_INST(Rigidbody) {
	auto c = o->AddComponent<Rigidbody>();

	ApplyRigidbody(c);
}

CE_PR_IMPL_COMP_APP(Rigidbody) {

}

CE_END_NAMESPACE
