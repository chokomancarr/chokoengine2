#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Camera) {
	CE_PR_ADD(orthographic);
	CE_PR_ADD(fov);
	CE_PR_ADD(orthoSize);
	CE_PR_ADD(nearClip);
	CE_PR_ADD(farClip);
	CE_PR_ADD(clearType);
	CE_PR_ADD(clearDepth);
	CE_PR_ADD(clearColor);
}

CE_PR_IMPL_COMP_INST(Camera) {
	auto c = o->AddComponent<Camera>();

	ApplyCamera(c);
}

CE_PR_IMPL_COMP_APP(Camera) {

}

CE_END_NAMESPACE
