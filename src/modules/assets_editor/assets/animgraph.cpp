#include "module_ae_inc.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(AnimGraph) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(_basePath + path));
	
	return ModuleAC::LoadAnimGraph(data);
}

CE_END_MOD_AE_NAMESPACE
