#include "chokoeditor.hpp"
#include "asset_loader.hpp"
#include "parsers/mesh.hpp"
#include "templates/meta/meta_common.hpp"

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

#define CE_E_MKM(nm) case EAssetType::nm: {\
    std::ofstream strm(ChokoEditor::assetPath + path + ".meta");\
    strm << meta::nm;\
    break;\
}

void EAssetLoader::GenDefaultMeta(const std::string& path, const EAssetType t) {
    switch (t) {
        CE_E_MKM(Mesh)
        CE_E_MKM(Texture)
    }
}

#define CE_E_LD(nm) case EAssetType::nm:\
    return static_cast<Asset>(Load ## nm(path));\
    break;

Asset EAssetLoader::Load(const std::string& path, const EAssetType t) {
    switch (t) {
        //CE_E_LD(Material)
        CE_E_LD(Mesh)
        //CE_E_LD(Shader)
        CE_E_LD(Texture)
    }
}

#define CE_E_IMPL(nm) nm EAssetLoader::Load ## nm(const std::string& path)

CE_E_IMPL(Mesh) {
    auto meta = LoadMeta(path);
    auto ext = StrExt::ExtensionOf(path);
    if (ext == "obj") {
        return MeshLoader::LoadObj(ChokoEditor::assetPath + path);
    }
}

CE_E_IMPL(Shader) {
    auto meta = LoadMeta(path);
    std::string vs, fs;
    auto data = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
    for (auto& d : data.group) {
        if (d.key.string == "vertex")
            vs = d.value.string;
        else if (d.key.string == "fragment")
            fs = d.value.string;
    }
    return Shader::New(vs, fs);
}

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