#include "module_ae_inc.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(AnimClip) {
	return ModuleAC::LoadAnimClip(_basePath + path);
}

CE_END_MOD_AE_NAMESPACE
