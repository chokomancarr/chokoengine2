#include "module_aa_inc.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(Prefab) {
	const auto meta = ArchiveParser::GetMetaOf(path);
	const auto data = JsonParser::Parse(ArchiveParser::ReadStr(path));
	return Prefab::New(data, [](const std::string& s) -> Prefab {
		return Get<Prefab>(AssetType::Prefab, s, true);
	});
}

CE_END_MOD_AA_NAMESPACE
