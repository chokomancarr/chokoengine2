#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(Texture) {
	const auto meta = LoadMeta(path);
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
	return Texture::New(_basePath + path, opts, async);
}

CE_END_MOD_AE_NAMESPACE
