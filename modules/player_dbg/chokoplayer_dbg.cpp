#include "chokoplayer_dbg.hpp"

CE_BEGIN_PL_NAMESPACE

std::string ChokoPlayerD::projectPath = "";

void ChokoPlayerD::Init() {
    ChokoPlayer::Init();

    projectPath = IO::path();
    for (int a = 0; a < 4; a++) {
        projectPath = projectPath.substr(0, 
            projectPath.find_last_of('/'));
    }
    projectPath += '/';
    Debug::Message("ChokoPlayerD", "Project root is: " + projectPath);
}

void ChokoPlayerD::Main() {
    ChokoPlayer::Main();
}

CE_END_PL_NAMESPACE
