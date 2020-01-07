#include "chokoeditor.hpp"

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
	//ECallbackManager::Register(CallbackSig::GLOBAL_BUILD, BuildRelease);
}

bool EProjectBuilder::BuildDebug() {
    while (busy());
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _busy = true;
    }
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
    dbgProgPath = CE_DIR_SYSTEM + "build/bin/chokoeditor_project" CE_PROG_EXT;
    return true;
}

bool EProjectBuilder::BuildRelease() {
    CE_NOT_IMPLEMENTED
}

CE_END_ED_NAMESPACE