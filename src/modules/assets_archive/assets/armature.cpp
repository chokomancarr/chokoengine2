#include "module_aa_inc.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(Armature) {
	const auto meta = ArchiveParser::GetMetaOf(path);
	const auto data = JsonParser::Parse(ArchiveParser::ReadStr(path));

	return ModuleAC::LoadArmature(data);
}

CE_END_MOD_AA_NAMESPACE
