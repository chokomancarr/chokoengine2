#include "chokoeditor.hpp"
#include "asset/archiver/getdeps.hpp"

#ifdef PLATFORM_WIN
    #define CE_PROG_EXT ".exe"
#else
    #define CE_PROG_EXT 
#endif

CE_BEGIN_ED_NAMESPACE

std::mutex EProjectBuilder::_mtx;
bool EProjectBuilder::_busy = false;

std::string EProjectBuilder::dbgProgPath = "";
std::string EProjectBuilder::relProgPath = "";

bool EProjectBuilder::busy() {
    std::lock_guard<std::mutex> lock(_mtx);
    return _busy;
}

void EProjectBuilder::Init() {
	ECallbackManager::Register(CallbackSig::GLOBAL_BUILD_DEBUG, CbFnFrom(BuildDebug));
	ECallbackManager::Register(CallbackSig::GLOBAL_BUILD_RELEASE, CbFnFrom(BuildRelease));

    dbgProgPath = CE_DIR_SYSTEM + "build/bin/chokoeditor_project" CE_PROG_EXT;
}

bool EProjectBuilder::BuildDebug() {
    while (busy());
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _busy = true;
    }
    EScriptLoaderGen::Generate();
    std::thread([](){
        Subprocess::ProcessInfo info;
        info.program = IO::path() + TaskList::builderDebug;
        info.args = {
            "--root", ChokoEditor::projectRoot,
            "--configArgs", //none
            "--clean", "0"
        };
        Subprocess::Run(info);
        std::lock_guard<std::mutex> lock(_mtx);
        _busy = false;
    }).detach();
    return true;
}

namespace {
	template <typename T>
	inline void writefile(std::ofstream& strm, const std::vector<T>& data) {
		strm.write((const char*)data.data(), data.size());
	}
	template <typename T>
	inline void writefile(std::ofstream& strm, const T* data, const size_t n) {
		strm.write((const char*)data, n);
	}
}

bool EProjectBuilder::BuildRelease() {
	while (busy());
	{
		std::lock_guard<std::mutex> lock(_mtx);
		_busy = true;
	}

	AssetDepends::Clear();
	for (auto& s : ESceneManager::scenes) {
		if (s.enabled) {
			auto sc = ESceneManager::GetSceneData(s.sig);
			AssetDepends::Reg(sc.obj);
			AssetDepends::RegBackground(sc.sky);
		}
	}

	const auto dir = ChokoEditor::projectRoot + "build/";
	EAssetArchiver::Exec(dir);
	const auto& index = ESceneManager::GetRelSceneIndex();
	std::ofstream strm(dir + "scene_index", std::ios::binary);
	writefile(strm, index.c_str(), index.size());
	const auto& scenes = ESceneManager::GetRelScenes();
	int i = 0;
	for (auto& s : scenes) {
		std::ofstream strm2(dir + "scene" + std::to_string(i) + ".dat", std::ios::binary);
		writefile(strm2, s);
	}

	std::thread([]() {
		Subprocess::ProcessInfo info;
		info.program = IO::path() + TaskList::builderRelease;
		info.args = {
			"--root", ChokoEditor::projectRoot,
			"--appname", "app"
		};
		Subprocess::Run(info);
		std::lock_guard<std::mutex> lock(_mtx);
		_busy = false;
	}).detach();
	return true;
}

CE_END_ED_NAMESPACE