#include "asset_loader.hpp"
#include "../vfs/vfs.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(Background) {
	const auto meta = ArchiveParser::GetMetaOf(path);

	auto strm = ArchiveParser::GetStrm(path);

	return Background::New(strm.strm, strm.len, 5, async);
}

CE_END_MOD_AA_NAMESPACE
