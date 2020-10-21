#include "module_aa_inc.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(AnimClip) {
	return ModuleAC::LoadAnimClip(ArchiveParser::GetStrm(path));
}

CE_END_MOD_AA_NAMESPACE
