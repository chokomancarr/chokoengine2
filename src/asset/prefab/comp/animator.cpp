#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Animator) {
	CE_PR_ADD(single);
	CE_PR_ADD(repeat);

	CE_PR_ADD(clip);
	CE_PR_ADD(graph);
}

CE_PR_IMPL_COMP_INST(Animator) {
	auto c = o->AddComponent<Animator>();

	CE_PR_SET(single);
	CE_PR_SET(repeat);

	CE_PR_SET_A(clip, AnimClip);
	CE_PR_SET_A(graph, AnimGraph);
}

CE_END_NAMESPACE
