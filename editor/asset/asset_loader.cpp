#include "chokoeditor.hpp"
#include "asset_loader.hpp"

CE_BEGIN_ED_NAMESPACE

JsonObject EAssetLoader::LoadMeta(const std::string& path) {
    auto obj = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path + ".meta"));
    if (obj.group[0].key.string != "signature") {
        Debug::Error("Asset MetaData", "Signature entry missing!");
    }
    if (obj.group[0].value.string != CE_E_META_SIGNATURE) {
        Debug::Error("Asset MetaData", "Signature is incorrect!");
    }
    if (obj.group[1].key.string != "version") {
        Debug::Error("Asset MetaData", "Version entry missing!");
    }
    //check for version compat here
    return obj;
}

Asset EAssetLoader::Load(const std::string& path, const EAssetType t) {
    switch (t) {
    case EAssetType::Texture:
        return static_cast<Asset>(LoadTexture(path));
        break;
    }
}

#define CE_E_IMPL(nm) nm EAssetLoader::Load ## nm(const std::string& path)

CE_E_IMPL(Texture) {
    auto meta = LoadMeta(path);
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
    return Texture::New(ChokoEditor::assetPath + path, opts);
}

CE_END_ED_NAMESPACE