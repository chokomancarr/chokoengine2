#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(LightProbe) {
	//CE_PR_ADD
}

CE_PR_IMPL_COMP_INST(LightProbe) {
	auto c = o->AddComponent<LightProbe>();

	ApplyLightProbe(c);
}

CE_PR_IMPL_COMP_APP(LightProbe) {
	//CE_PR_SET
}

CE_END_NAMESPACE
