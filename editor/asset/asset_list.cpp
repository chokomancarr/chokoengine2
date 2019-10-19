#include "chokoeditor.hpp"
#include "asset_loader.hpp"
#ifndef PLATFORM_WIN
#include <utime.h>
#endif

CE_BEGIN_ED_NAMESPACE

EAssetList::_Entry::_Entry(const std::string& sig)
    : sig(sig), obj(nullptr), modtime(0) {}


EAssetList::_ScriptEntry::_ScriptEntry(const std::string& scr)
	: sig(sig), info({}), modtime(0) {}


std::array<std::vector<EAssetList::_Entry>, (int)EAssetType::_COUNT> EAssetList::_entries = {};
std::array<std::vector<std::string>, (int)EAssetType::_COUNT> EAssetList::_exts = {};

std::array<std::vector<std::string>, (int)EExportType::_COUNT> EAssetList::_export_exts = {};
std::vector<EAssetList::_ScriptEntry> EAssetList::_scriptEntries;

bool EAssetList::Scan_Fd(const std::string& fd) {
    bool dirty = false;
    const auto ffd = ChokoEditor::assetPath + fd;
    auto fls = IO::ListFiles(ffd);
    for (auto& f : fls) {
        const auto ext = StrExt::ExtensionOf(f);
        const auto sig = fd + f;
		if (ext == "hpp") {
			auto& ent = _scriptEntries;
			auto it = std::find_if(ent.begin(), ent.end(), [&](const _ScriptEntry& _e) {
				return _e.sig == sig;
			});
			if (it == ent.end()) {
				Debug::Message("AssetList", "Registered " + sig, TerminalColor::BrightGreen);
				ent.push_back(_ScriptEntry(sig));
				it = ent.end() - 1;
				it->sig = sig;
				it->modtime = 0;
			}
			const auto mt = IO::ModTime(ffd + f);
			if (it->modtime < mt) {
				Debug::Message("AssetList", "Updating " + sig, TerminalColor::BrightCyan);
				it->info = Scripting::ParseInfo(ffd + f);
				if (!IO::FileExists(ffd + f + ".meta")) {
					EAssetLoader::GenDefaultScriptMeta(sig);
				}
				it->modtime = mt;
				//dirty = true;
			}
			goto next;
		}
        for (int a = 0; a < (int)EAssetType::_COUNT; a++) {
            for (auto& e : _exts[a]) {
                if (e == ext) {
                    auto& ent = _entries[a];
                    auto it = std::find_if(ent.begin(), ent.end(), [&](const _Entry& _e) {
                        return _e.sig == sig;
                    });
                    if (it == ent.end()) {
                        Debug::Message("AssetList", "Registered " + sig, TerminalColor::BrightGreen);
                        ent.push_back(_Entry(sig));
                        it = ent.end() - 1;
                        it->modtime = 0;
                    }
                    const auto mt = IO::ModTime(ffd + f);
                    if (it->modtime < mt) {
                        Debug::Message("AssetList", "Updating " + sig, TerminalColor::BrightCyan);
                        if (!!it->obj) {
                            it->obj = EAssetLoader::Load(sig, (EAssetType)a);
                            //it->obj->dirty(true);
                        }
                        if (!IO::FileExists(ffd + f + ".meta")) {
                            EAssetLoader::GenDefaultMeta(sig, (EAssetType)a);
                        }
                        it->modtime = mt;
                        //dirty = true;
                    }
                    goto next;
                }
            }
        }
        for (int a = 0; a < (int)EExportType::_COUNT; a++) {
            for (auto& e : _export_exts[a]) {
                if (e == ext) {
                    const auto mt = IO::ModTime(ffd + f);
                    const auto mnm = ffd + f + ".meta";
                    const auto mmt = IO::ModTime(mnm);
                    if (mmt < mt) {
                        Debug::Message("AssetList", "Processing " + sig, TerminalColor::BrightYellow);
                        if (!IO::FileExists(mnm)) {
                            EAssetLoader::GenDefaultMeta(sig, (EExportType)a);
                            UpdateModTime(mnm, false);
                        }
                        if (EAssetLoader::Load(sig, (EExportType)a)) {
                            UpdateModTime(mnm, true);
                            dirty = true;
                        }
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

void EAssetList::UpdateModTime(const std::string& fl, bool now) {
#ifdef PLATFORM_WIN
	auto hndl = CreateFile(fl.c_str(), FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	FILETIME ft = {};
	if (now) {
		SYSTEMTIME st;
		GetSystemTime(&st);              // Gets the current system time
		SystemTimeToFileTime(&st, &ft);  // Converts the current system time to file time format
	}
	SetFileTime(hndl, nullptr, nullptr, &ft);

	CloseHandle(hndl);
#else
    if (now) utime(fl.c_str(), nullptr);
    else {
        utimbuf buf = {};
        utime(fl.c_str(), &buf);
    }
#endif
}

void EAssetList::Init() {
    _exts[(int)EAssetType::Armature] = { "armature" };
    _exts[(int)EAssetType::Material] = { "material" };
    _exts[(int)EAssetType::Mesh] = { "obj", "mesh" };
    _exts[(int)EAssetType::Shader] = { "shader" };
	_exts[(int)EAssetType::VShader] = { "visualshader" };
    _exts[(int)EAssetType::Texture] = { "png", "jpg", "bmp" };
    _exts[(int)EAssetType::SceneObject] = { "prefab" };

    _export_exts[(int)EExportType::Model] = { "blend" };
    _export_exts[(int)EExportType::Image] = { "psd" };
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

const Asset& EAssetList::Get(EAssetType t, const std::string& sig, bool async) {
    auto& ar = _entries[(int)t];
    auto i = std::find_if(ar.begin(), ar.end(), [&](const _Entry& e) {
        return e.sig == sig;
    });
    if (i == ar.end()) {
        Debug::Error("AssetList", "Cannot find asset \"" + sig + "\" of type " + std::to_string((int)t) + "!");
        static Asset a(nullptr);
        return a;
    }
    if (!i->obj) {
        Debug::Message("AssetList", "Loading " + i->sig, TerminalColor::BrightYellow);
        i->obj = EAssetLoader::Load(i->sig, t, async);
    }
	else if (!async) {
		i->obj->Wait();
	}
	return i->obj;
}

const ScriptInfo& EAssetList::GetScr(const std::string& sig) {
	auto& ar = _scriptEntries;
	auto i = std::find_if(ar.begin(), ar.end(), [&](const _ScriptEntry& e) {
		return e.sig == sig;
	});
	if (i == ar.end()) {
		Debug::Error("AssetList", "Cannot find script \"" + sig + "\!");
		return nullptr;
	}
	return i->info;
}

EAssetList::TypeOfSt EAssetList::TypeOf(const std::string& f) {
	const auto ext = StrExt::ExtensionOf(f);
	for (int a = 0; a < (int)EAssetType::_COUNT; a++) {
		for (auto& e : _exts[a]) {
			if (e == ext) {
				return TypeOfSt{ false, (EAssetType)a, EExportType::Unknown };
			}
		}
	}
	for (int a = 0; a < (int)EExportType::_COUNT; a++) {
		for (auto& e : _exts[a]) {
			if (e == ext) {
				return TypeOfSt{ true, EAssetType::Unknown, (EExportType)a };
			}
		}
	}
	return TypeOfSt{ true, EAssetType::Unknown, EExportType::Unknown };
}

CE_END_ED_NAMESPACE