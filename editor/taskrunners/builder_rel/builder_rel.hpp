#pragma once
#include "chokoengine.hpp"

/* Directory structure
 * project_root/
 * build/
 *   [appname].exe
 */

class BuilderRelease {
public:
    static std::string projectRoot;
	static std::string cmakePath;
	static std::vector<std::string> cmakeBuildArgs;
	static std::string appName;

    static int Run();
};
