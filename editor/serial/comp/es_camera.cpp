#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_ES_IMPL_COMP(Camera) {
	CE_ES_ADD(orthographic);
	CE_ES_ADD(fov);
	CE_ES_ADD(orthoSize);
	CE_ES_ADD(nearClip);
	CE_ES_ADD(farClip);
	CE_ES_ADD(clearType);
	CE_ES_ADD(clearDepth);
	CE_ES_ADD(clearColor);
}

CE_ES_IMPL_COMP_INST(Camera) {
	auto& c = o->AddComponent<Camera>();

}

CE_END_ED_NAMESPACE