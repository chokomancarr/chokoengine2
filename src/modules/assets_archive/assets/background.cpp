#include "asset_loader.hpp"
#include "../vfs/vfs.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(Background) {
	const auto meta = ArchiveParser::GetMetaOf(path);

	return Background::New(ArchiveParser::GetStrm(path), 5, async);
}

CE_END_MOD_AA_NAMESPACE
