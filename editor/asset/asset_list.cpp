#include "chokoeditor.hpp"
#ifndef PLATFORM_WIN
#include <utime.h>
#endif

CE_BEGIN_ED_NAMESPACE

EAssetList::_Entry::_Entry(const std::string& sig, const std::string& gsig)
    : sig(sig), genSig(gsig), obj(nullptr), modtime(0) {}

EAssetList::_ScriptEntry::_ScriptEntry(const std::string& scr)
	: sig(scr), info({}), modtime(0) {}


namespace {
	std::array<std::vector<std::string>, (int)AssetType::_COUNT> _exts;
	std::array<std::vector<std::string>, (int)EExportType::_COUNT> _export_exts;
	std::array<std::vector<std::string>, (int)EExtType::_COUNT> _other_exts;

	typedef std::pair<AssetType, std::string> _ast;
	const std::unordered_map<std::string, _ast> _expRedirExts = {
		{"blend", _ast(AssetType::Prefab, ".prefab")},
		{"psd", _ast(AssetType::Texture, ".png")},
	};

	void UpdateModTime(const std::string& fl, bool now) {
#ifdef PLATFORM_WIN
		auto hndl = CreateFile(fl.c_str(), FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		FILETIME ft = {};
		if (now) {
			SYSTEMTIME st;
			GetSystemTime(&st);              // Gets the current system time
			SystemTimeToFileTime(&st, &ft);  // Converts the current system time to file time format
		}
		else {
			ULARGE_INTEGER i;
			i.QuadPart = 1;
			ft.dwLowDateTime = i.LowPart;
			ft.dwHighDateTime = i.HighPart;
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

	template <typename T>
	class regsig {
	public:
		template <typename E>
		static void invoke(const std::string&, typename std::vector<T>::iterator) {}
		template <>
		static void invoke<EExportType>(const std::string& sig, typename std::vector<T>::iterator it) {
			/*
			const auto dr = ".exported/" + sig + "/";
			const auto lf = IO::ListFiles(CE_DIR_ASSET + dr);
			if (lf.size() > 0) {
				if (lf.size() == 1) {
					it->genSig = dr + lf[0];
				}
				else {
					const auto df = CE_DIR_ASSET + dr + "default";
					if (IO::FileExists(df)) {
						it->genSig = dr + IO::ReadFile(df);
					}
					else return;
				}
			}
			else return;
			*/
			const auto& rd = _expRedirExts.at(StrExt::ExtensionOf(sig));
			it->genType = rd.first;
			it->genSig = ".exported/" + sig + "/" + StrExt::RemoveFd(sig) + rd.second;
			bool hasGen = IO::FileExists(CE_DIR_ASSET + it->genSig);
			EDebug::Log("AssetList", "             -> " + it->genSig, 0);
		}
	};

	template <typename E, typename T>
	void scanone(std::vector<T>& ent, const std::string& sig, const std::string& path, E e,
			const std::function<void(T&, bool)> fn) {
		auto it = std::find_if(ent.begin(), ent.end(), [&](const T& e) {
			return e.sig == sig;
		});
		if (it == ent.end()) { //new asset in list
			EDebug::Log("AssetList", "Registered " + sig, 0);
			ent.push_back(T(sig));
			it = ent.end() - 1;
			regsig<T>::template invoke<E>(sig, it);
		}
		const auto mpath = path + ".meta";
		const auto mt = IO::ModTime(path);
		bool metaold;
		if (!IO::FileExists(mpath)) { //no metadata
			EDebug::Log("AssetList", "Updating " + sig, 0);
			EAssetLoader::GenDefaultMeta(sig, e);
			UpdateModTime(mpath, false);
			metaold = true;
			fn(*it, true);
			it->modtime = mt;
		}
		else {
			metaold = (IO::ModTime(mpath) < mt); //file is newer than metadata
			if (it->modtime < mt || metaold) { //or file is newer than list
				EDebug::Log("AssetList", "Updating " + sig, 0);
				fn(*it, metaold);
				it->modtime = mt;
			}
		}
		if (metaold)
			UpdateModTime(mpath, true);
	}
	
	template <typename E, typename T, size_t N>
	bool doscan(const std::string& sig, const std::string& path, const std::string& ext, std::array<std::vector<T>, N>& ets,
			const std::array<std::vector<std::string>, N>& exts, const std::function<void(E, T&, bool)> fn) {
		for (int a = 0; a < N; a++) {
			for (auto& e : exts[a]) {
				if (e == ext) {
					scanone<E, T>(ets[a], sig, path, (E)a, [&](T& v, bool b) {
						fn((E)a, v, b);
					});
					return true;
				}
			}
		}
		return false;
	}
}

std::array<std::vector<EAssetList::_Entry>, (int)AssetType::_COUNT> EAssetList::_entries = {};
std::array<std::vector<EAssetList::_Entry>, (int)EExportType::_COUNT> EAssetList::_exportEntries = {};
std::array<std::vector<EAssetList::_Entry>, (int)EExtType::_COUNT> EAssetList::_otherEntries = {};
std::vector<EAssetList::_ScriptEntry> EAssetList::_scriptEntries;

bool EAssetList::Scan_Fd(const std::string& fd) {
    bool dirty = false; //do we need another iteration?
    const auto ffd = CE_DIR_ASSET + fd;
    auto fls = IO::ListFiles(ffd);
    for (auto& f : fls) {
        const auto ext = StrExt::ExtensionOf(f);
        const auto sig = fd + f;
		const auto path = ffd + f;
		if (ext == "hpp") {
			scanone<EExtType, _ScriptEntry>(_scriptEntries, sig, path, EExtType::ScrHeader, [&](_ScriptEntry& v, bool) {
				v.info = EScripting::ParseInfo(sig);
			});
		}
		else if (doscan<AssetType, _Entry>(sig, path, ext, _entries, _exts, [&](AssetType t, _Entry& e, bool b) {
			if (b && !!e.obj) {
				e.obj = EAssetLoader::Load(sig, t);
			}
		})) {}
		else if (doscan<EExportType, _Entry>(sig, path, ext, _exportEntries, _export_exts, [&](EExportType t, _Entry& e, bool b) {
			if (b && EAssetLoader::Load(sig, t)) { //only reexport if file changed
				dirty = true;
			}
		})) {}
		else if (doscan<EExtType, _Entry>(sig, path, ext, _otherEntries, _other_exts, [&](EExtType t, _Entry& e, bool) {
			
		})) {}
    }

    auto drs = IO::ListDirectories(ffd);
    for (auto& d : drs) {
        dirty |= Scan_Fd(fd + d + "/");
    }
    return dirty;
}

void EAssetList::Init() {
	_exts[(int)AssetType::AnimClip] = { "animclip" };
	_exts[(int)AssetType::AnimGraph] = { "animgraph" };
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
		EDebug::Log("AssetList", "Scanning directories: pass " + std::to_string(pass));
		dirty = Scan_Fd("");
    }
    while(dirty && ++pass < 5);

	if (pass == 5) {
		EDebug::Log("AssetList", "too many import iterations! aborting", 2);
	}
}

void EAssetList::Reimport(const std::string& sig) {
	UpdateModTime(CE_DIR_ASSET + sig + ".meta", false);
	Rescan();
}

const Asset& EAssetList::Get(AssetType t, const std::string& sig, bool async) {
	if ((int)t >= (int)AssetType::_COUNT) {
		static Asset _null = nullptr;
		return _null;
	}
	auto& ar = _entries[(int)t];
	auto i = std::find_if(ar.begin(), ar.end(), [&](const _Entry& e) {
		return e.sig == sig;
	});
	if (i == ar.end()) {
		EDebug::Log("AssetList", "Cannot find asset \"" + sig + "\" of type " + std::to_string((int)t) + "!", EDEBUG_LVL_ERROR);
		static Asset a(nullptr);
		return a;
	}
	if (!i->obj) {
		EDebug::Log("AssetList", "Loading " + i->sig, 0);
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

Asset EAssetList::GetRedirected(const TypeOfSt& st, const std::string& sig, bool async) {
	switch (st.subtype) {
	case TypeOfSt::Type::Asset:
		return Get(st.assetType, sig, async);
	case TypeOfSt::Type::Export: {
		const auto& ee = _exportEntries[(int)st.exportType];
		auto it = std::find_if(ee.begin(), ee.end(), [sig](const _Entry& e) {
			return e.sig == sig;
		});
		if (it == ee.end()) return nullptr;
		return Get(it->genType, it->genSig, async);
	}
	default:
		return nullptr;
	}
}

const ScriptInfo& EAssetList::GetScr(const std::string& sig) {
	auto& ar = _scriptEntries;
	auto i = std::find_if(ar.begin(), ar.end(), [&](const _ScriptEntry& e) {
		return e.sig == sig;
	});
	if (i == ar.end()) {
		EDebug::Log("AssetList", "Cannot find script \"" + sig + "\"!", EDEBUG_LVL_ERROR);
		static ScriptInfo null = nullptr;
		return null;
	}
	return i->info;
}

EAssetList::TypeOfSt EAssetList::TypeOf(const std::string& f) {
	const auto ext = StrExt::ExtensionOf(f);
	if (ext == ".hpp") {
		return TypeOfSt{ TypeOfSt::Type::Asset, AssetType::Unknown, EExportType::Unknown, EExtType::ScrHeader };
	}
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
