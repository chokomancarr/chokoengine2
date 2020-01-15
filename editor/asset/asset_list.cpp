#include "chokoeditor.hpp"
#ifndef PLATFORM_WIN
#include <utime.h>
#endif

CE_BEGIN_ED_NAMESPACE

EAssetList::_Entry::_Entry(const std::string& sig)
    : sig(sig), obj(nullptr), modtime(0) {}

EAssetList::_Entry2::_Entry2(const std::string& sig)
	: sig(sig), modtime(0) {}

EAssetList::_ScriptEntry::_ScriptEntry(const std::string& scr)
	: sig(scr), info({}), modtime(0) {}


namespace {
	std::array<std::vector<std::string>, (int)AssetType::_COUNT> _exts;
	std::array<std::vector<std::string>, (int)EExportType::_COUNT> _export_exts;
	std::array<std::vector<std::string>, (int)EExtType::_COUNT> _other_exts;

	void UpdateModTime(const std::string& fl, bool now) {
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

	template <typename E, typename T>
	void scanone(std::vector<T>& ent, const std::string& sig, const std::string& path, E e,
			const std::function<void(T&)> fn) {
		auto it = std::find_if(ent.begin(), ent.end(), [&](const T& e) {
			return e.sig == sig;
		});
		if (it == ent.end()) {
			Debug::Message("AssetList", "Registered " + sig, TerminalColor::BrightGreen);
			ent.push_back(T(sig));
			it = ent.end() - 1;
			it->sig = sig;
			it->modtime = 0;
		}
		const auto mt = IO::ModTime(path);
		if (it->modtime < mt) {
			Debug::Message("AssetList", "Updating " + sig, TerminalColor::BrightCyan);
			if (!IO::FileExists(path + ".meta")) {
				EAssetLoader::GenDefaultMeta(sig, e);
				UpdateModTime(path + ".meta", false);
			}
			//this function is called for first pass / when object is updated while in background
			fn(*it);
			it->modtime = mt;
			UpdateModTime(path + ".meta", true);
		}
	}
	
	template <typename E, typename T, size_t N>
	bool doscan(const std::string& sig, const std::string& path, const std::string& ext, std::array<std::vector<T>, N>& ets,
			const std::array<std::vector<std::string>, N>& exts, const std::function<void(E, T&)> fn) {
		for (int a = 0; a < N; a++) {
			for (auto& e : exts[a]) {
				if (e == ext) {
					/*
					auto& ent = ets[a];
					auto it = std::find_if(ent.begin(), ent.end(), [&](const T& _e) {
						return _e.sig == sig;
					});
					if (it == ent.end()) {
						Debug::Message("AssetList", "Registered " + sig, TerminalColor::BrightGreen);
						ent.push_back(T(sig));
						it = ent.end() - 1;
						it->modtime = IO::ModTime(path + ".meta");
					}
					const auto mt = IO::ModTime(path);
					if (it->modtime < mt) {
						Debug::Message("AssetList", "Updating " + sig, TerminalColor::BrightCyan);
						if (!IO::FileExists(path + ".meta")) {
							EAssetLoader::GenDefaultMeta(sig, (E)a);
							UpdateModTime(path + ".meta", false);
						}
						//this function is called for first pass / when object is updated while in background
						fn((E)a, *it);
						it->modtime = mt;
						UpdateModTime(path + ".meta", true);
					}
					*/
					scanone<E, T>(ets[a], sig, path, (E)a, [&](T& v) {
						fn((E)a, v);
					});
					return true;
				}
			}
		}
		return false;
	}
}

std::array<std::vector<EAssetList::_Entry>, (int)AssetType::_COUNT> EAssetList::_entries = {};
std::array<std::vector<EAssetList::_Entry2>, (int)EExportType::_COUNT> EAssetList::_exportEntries = {};
std::array<std::vector<EAssetList::_Entry2>, (int)EExtType::_COUNT> EAssetList::_otherEntries = {};
std::vector<EAssetList::_ScriptEntry> EAssetList::_scriptEntries;

bool EAssetList::Scan_Fd(const std::string& fd) {
    bool dirty = false;
    const auto ffd = CE_DIR_ASSET + fd;
    auto fls = IO::ListFiles(ffd);
    for (auto& f : fls) {
        const auto ext = StrExt::ExtensionOf(f);
        const auto sig = fd + f;
		const auto path = ffd + f;
		if (ext == "hpp") {
			scanone<EExtType, _ScriptEntry>(_scriptEntries, sig, path, EExtType::ScrHeader, [&](_ScriptEntry& v) {
				v.info = EScripting::ParseInfo(sig);
			});
			continue;
		}
		if (doscan<AssetType, _Entry>(sig, path, ext, _entries, _exts, [sig](AssetType t, _Entry& e) {
			if (!!e.obj) {
				e.obj = EAssetLoader::Load(sig, t);
			}
		})) continue;
		
		if (doscan<EExportType, _Entry2>(sig, path, ext, _exportEntries, _export_exts, [&](EExportType t, _Entry2& e) {
			if (EAssetLoader::Load(sig, t)) {
				dirty = true;
			}
		})) continue;
		/*
		for (int a = 0; a < (int)EExtType::_COUNT; a++) {
			for (auto& e : _other_exts[a]) {
				if (e == ext) {
					auto& ent = _otherEntries[a];
					auto it = std::find_if(ent.begin(), ent.end(), [&](const _Entry2& _e) {
						return _e.sig == sig;
					});
					if (it == ent.end()) {
						Debug::Message("AssetList", "Registered " + sig, TerminalColor::BrightGreen);
						ent.push_back(_Entry2(sig));
						it = ent.end() - 1;
						it->modtime = 0;
					}
					const auto mt = IO::ModTime(path);
					if (it->modtime < mt) {
						Debug::Message("AssetList", "Updating " + sig, TerminalColor::BrightCyan);
						if (!IO::FileExists(path + ".meta")) {
							EAssetLoader::GenDefaultMeta(sig, (EExportType)a);
							UpdateModTime(path + ".meta", false);
						}
						if (EAssetLoader::Load(sig, (EExportType)a)) {
							UpdateModTime(path + ".meta", true);
							dirty = true;
						}
						it->modtime = mt;
					}
					goto next;
				}
			}
		}
		*/
		if (doscan<EExtType, _Entry2>(sig, path, ext, _otherEntries, _other_exts, [&](EExtType t, _Entry2& e) {
			
		})) continue;
    }

    auto drs = IO::ListDirectories(ffd);
    for (auto& d : drs) {
        dirty |= Scan_Fd(fd + d + "/");
    }
    return dirty;
}

void EAssetList::Init() {
    _exts[(int)AssetType::Armature] = { "armature" };
    _exts[(int)AssetType::Background] = { "hdr", "exr" };
    _exts[(int)AssetType::Material] = { "material" };
    _exts[(int)AssetType::Mesh] = { "obj", "mesh" };
    _exts[(int)AssetType::Shader] = { "shader" };
    _exts[(int)AssetType::Texture] = { "png", "jpg", "bmp" };
    _exts[(int)AssetType::Prefab] = { "prefab" };

    _export_exts[(int)EExportType::Model] = { "blend" };
    _export_exts[(int)EExportType::Image] = { "psd" };

	_other_exts[(int)EExtType::Scene] = { "scene" };
	_other_exts[(int)EExtType::ScrSource] = { "cpp" };
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

const Asset& EAssetList::Get(AssetType t, const std::string& sig, bool async) {
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

std::vector<std::string> EAssetList::GetList(AssetType t) {
	std::vector<std::string> res;
	for (auto& e : _entries[(int)t]) {
		res.push_back(e.sig);
	}
	return res;
}

std::vector<std::string> EAssetList::GetScrList() {
	std::vector<std::string> res;
	for (auto& e : _scriptEntries) {
		res.push_back(e.sig);
	}
	return res;
}

Asset EAssetList::Get2(AssetType t, const std::string& s) {
	return Get(t, s, true);
}

const ScriptInfo& EAssetList::GetScr(const std::string& sig) {
	auto& ar = _scriptEntries;
	auto i = std::find_if(ar.begin(), ar.end(), [&](const _ScriptEntry& e) {
		return e.sig == sig;
	});
	if (i == ar.end()) {
		Debug::Error("AssetList", "Cannot find script \"" + sig + "\"!");
		static ScriptInfo null = nullptr;
		return null;
	}
	return i->info;
}

EAssetList::TypeOfSt EAssetList::TypeOf(const std::string& f) {
	const auto ext = StrExt::ExtensionOf(f);
	for (int a = 0; a < (int)AssetType::_COUNT; a++) {
		for (auto& e : _exts[a]) {
			if (e == ext) {
				return TypeOfSt{ TypeOfSt::Type::Asset, (AssetType)a, EExportType::Unknown, EExtType::Unknown };
			}
		}
	}
	for (int a = 0; a < (int)EExportType::_COUNT; a++) {
		for (auto& e : _export_exts[a]) {
			if (e == ext) {
				return TypeOfSt{ TypeOfSt::Type::Export, AssetType::Unknown, (EExportType)a, EExtType::Unknown };
			}
		}
	}
	for (int a = 0; a < (int)EExtType::_COUNT; a++) {
		for (auto& e : _other_exts[a]) {
			if (e == ext) {
				return TypeOfSt{ TypeOfSt::Type::Other, AssetType::Unknown, EExportType::Unknown, (EExtType)a };
			}
		}
	}
	return TypeOfSt{ TypeOfSt::Type::Unknown, AssetType::Unknown, EExportType::Unknown, EExtType::Unknown };
}

CE_END_ED_NAMESPACE
