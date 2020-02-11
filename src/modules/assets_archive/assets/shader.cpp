#include "module_aa_inc.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(Shader) {
	const auto meta = ArchiveParser::GetMetaOf(path);
	const auto data = JsonParser::Parse(ArchiveParser::ReadStr(path));
	return ModuleAC::LoadShader(data);
}

CE_END_MOD_AA_NAMESPACE
