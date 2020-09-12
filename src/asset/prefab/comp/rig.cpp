#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Rig) {

}

CE_PR_IMPL_COMP_INST(Rig) {
	auto c = o->AddComponent<Rig>();

	ApplyRig(c);
}

CE_PR_IMPL_COMP_APP(Rig) {
	CE_PR_SET_A(armature, Armature);
}

CE_END_NAMESPACE
