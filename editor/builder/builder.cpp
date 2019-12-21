#include "chokoeditor.hpp"

#ifdef PLATFORM_WIN
    #define CE_PROG_EXT ".exe"
#else
    #define CE_PROG_EXT 
#endif

CE_BEGIN_ED_NAMESPACE

std::string EProjectBuilder::dbgProgPath = "";
std::string EProjectBuilder::relProgPath = "";

void EProjectBuilder::Init() {

}

bool EProjectBuilder::BuildDebug() {
    Subprocess::Run(IO::path() + TaskList::builderDebug,
        {
            "--root", StrExt::ParentFd(ChokoEditor::assetPath),
            "--configArgs", //none
            "--clean", "0"
        }, 0
    );
    dbgProgPath = StrExt::ParentFd(ChokoEditor::assetPath) + "system/build/bin/chokoeditor_project" CE_PROG_EXT;
    return true;
}

bool EProjectBuilder::BuildRelease() {
    CE_NOT_IMPLEMENTED
}

CE_END_ED_NAMESPACE