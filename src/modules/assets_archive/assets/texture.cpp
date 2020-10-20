#include "module_aa_inc.hpp"

CE_BEGIN_MOD_AA_NAMESPACE

CE_MOD_AA_IMPL(Texture) {
	const auto meta = ArchiveParser::GetMetaOf(path);
	auto opts = TextureOptions();
	for (auto& g : meta.group) {
		if (g.key.string == "xwrap")
			opts.xwrap = g.value.ToEnum<TextureWrap>({ "Clamp", "Repeat", "Mirror" });
		else if (g.key.string == "ywrap")
			opts.ywrap = g.value.ToEnum<TextureWrap>({ "Clamp", "Repeat", "Mirror" });
		else if (g.key.string == "mipmaps")
			opts.mipmaps = std::stoi(g.value.string);
		else if (g.key.string == "linear")
			opts.linear = g.value.ToBool();
	}

	auto strm = ArchiveParser::GetStrm(path);
	return Texture::New(strm.strm, strm.len, path.substr(path.find_last_of('.') + 1), opts, async);
}

CE_END_MOD_AA_NAMESPACE
