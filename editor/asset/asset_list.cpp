#include "chokoeditor.hpp"
#include "asset_loader.hpp"

CE_BEGIN_ED_NAMESPACE

EAssetList::_Entry::_Entry(const std::string& sig)
    : sig(sig), obj(nullptr), modtime(0) {}


std::array<std::vector<EAssetList::_Entry>, (int)EAssetType::_COUNT> EAssetList::_entries = {};
std::array<std::vector<std::string>, (int)EAssetType::_COUNT> EAssetList::_exts = {};

bool EAssetList::Scan_Fd(const std::string& fd) {
    bool dirty = false;
    const auto ffd = ChokoEditor::assetPath + fd;
    auto fls = IO::ListFiles(ffd);
    for (auto& f : fls) {
        auto ext = StrExt::ExtensionOf(f);
        for (int a = 0; a < (int)EAssetType::_COUNT; a++) {
            for (auto& e : _exts[a]) {
                if (e == ext) {
                    auto& ent = _entries[a];
                    const auto sig = fd + f;
                    auto it = std::find_if(ent.begin(), ent.end(), [&](const _Entry& _e) {
                        return _e.sig == sig;
                    });
                    if (it == ent.end()) {
                        Debug::Message("AssetList", "\033[0;32m" "Registered \"" + sig + "\"" "\033[0m");
                        ent.push_back(_Entry(sig));
                        it = ent.end() - 1;
                        it->modtime = 0;
                    }
                    const auto mt = IO::ModTime(ffd + f);
                    if (it->modtime < mt) {
                        Debug::Message("AssetList", "\033[0;36m" "Updating " + sig + "\033[0m");
                        if (!IO::FileExists(ffd + f + ".meta")) {
                            EAssetLoader::GenDefaultMeta(sig, (EAssetType)a);
                        }
                        if (!!it->obj) {
                            it->obj = EAssetLoader::Load(sig, (EAssetType)a);
                            //it->obj->dirty(true);
                        }
                        it->modtime = mt;
                        dirty = true;
                    }
                    goto next;
                }
            }
        }
        next:;
    }

    auto drs = IO::ListDirectories(ffd);
    for (auto& d : drs) {
        dirty |= Scan_Fd(fd + d + "/");
    }
    return dirty;
}

void EAssetList::Init() {
    _exts[(int)EAssetType::Armature] = { "armature" };
    _exts[(int)EAssetType::Material] = { "material" };
    _exts[(int)EAssetType::Mesh] = { "obj", "mesh" };
    _exts[(int)EAssetType::Shader] = { "shader" };
    _exts[(int)EAssetType::Texture] = { "png", "jpg", "bmp" };
    _exts[(int)EAssetType::SceneObject] = { "prefab" };
}

void EAssetList::Rescan() {
    bool dirty;
    int pass = 1;
    do {
        Debug::Message("AssetList", "Scanning directories: pass " + std::to_string(pass++));
        dirty = Scan_Fd("");
    }
    while(dirty);
    Debug::Message("AssetList", "Scanning complete.");
}

const Object& EAssetList::Get(EAssetType t, const std::string& sig) {
    auto& ar = _entries[(int)t];
    auto i = std::find_if(ar.begin(), ar.end(), [&](const _Entry& e) {
        return e.sig == sig;
    });
    if (i == ar.end()) {
        Debug::Error("AssetList", "Cannot find asset \"" + sig + "\" of type " + std::to_string((int)t) + "!");
        static Object a(nullptr);
        return a;
    }
    if (!i->obj) {
        Debug::Message("AssetList",  "\033[0;34m" "Loading \"" + i->sig + "\"" "\033[0m");
        i->obj = EAssetLoader::Load(i->sig, t);
    }
    return i->obj;
}

CE_END_ED_NAMESPACE