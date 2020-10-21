#include "module_aa_inc.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(AnimGraph) {
	const auto meta = ArchiveParser::GetMetaOf(path);
	const auto data = JsonParser::Parse(ArchiveParser::ReadStr(path));
	
	return ModuleAC::LoadAnimGraph(data);
}

CE_END_MOD_AA_NAMESPACE
