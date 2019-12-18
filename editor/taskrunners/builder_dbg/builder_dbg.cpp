#include "builder_dbg.hpp"

using namespace ChokoEngine;

std::string BuilderDebug::projectRoot = "";
std::vector<std::string> BuilderDebug::cmakeConfigArgs = {};
bool BuilderDebug::clean = false;

int BuilderDebug::Run() {
    const auto printer = [](const std::string& s) {
        std::cout << s << std::endl;
    };

    Debug::Message("Builder", "Generating debug binary for project \""
        + projectRoot + "\"");

    // --- generate script resolver ---

    // --- execute cmake ---

    if (clean) {
        IO::RemoveFile(projectRoot + "system/build/CMakeCache.txt");
    }

    Subprocess::ProcessInfo info;
    info.program = "cmake";
    info.workingDir = projectRoot + "system/build/";
    info.callback = printer;

    if (clean || !IO::FileExists(projectRoot + "system/build/CMakeCache.txt")) {
        info.args = cmakeConfigArgs;
        std::vector<std::string> args2 = {
            "../.."
        };
        info.args.insert(info.args.end(), args2.begin(), args2.end());

        if (Subprocess::Run(info) != 0) {
            return 1;
        }
    }

    info.args = {};
    std::vector<std::string> args2 = {
        "--build", "."
    };
    info.args.insert(info.args.end(), args2.begin(), args2.end());

    if (Subprocess::Run(info) != 0) {
        return 2;
    }

    return 0;
}