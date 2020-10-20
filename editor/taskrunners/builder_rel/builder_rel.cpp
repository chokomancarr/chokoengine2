#include "builder_rel.hpp"

using namespace ChokoEngine;

std::string BuilderRelease::projectRoot = "";
std::string BuilderRelease::cmakePath = "cmake";
std::vector<std::string> BuilderRelease::cmakeBuildArgs = {};
std::string BuilderRelease::appName;

int BuilderRelease::Run() {
    const auto printer = [](const std::string& s) {
        std::cout << s << std::endl;
    };

    Debug::Message("Builder", "Generating release binary for project \""
        + projectRoot + "\"");

    Subprocess::ProcessInfo info;
    info.program = cmakePath;
    info.workingDir = projectRoot + "system/build/";
	info.callback = printer;

    info.args = {
        "--build", ".", "--target", "chokoeditor_project_rel"
    };
    info.args.insert(info.args.end(), cmakeBuildArgs.begin(), cmakeBuildArgs.end());

    if (Subprocess::Run(info) != 0) {
        return 2;
    }

	//copy output exe

    return 0;
}