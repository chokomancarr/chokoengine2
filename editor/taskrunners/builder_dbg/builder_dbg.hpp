#pragma once
#include "chokoengine.hpp"

/* Directory structure
 * project_root/
 *   assets/
 *     .ce/
 *       startup.scene
 *     %other assets
 *   resources/
 *     default_font.ttf
 *   system/
 *     build/bin/
 *       debug_player.exe
 */

class BuilderDebug {
public:
    static std::string projectRoot;
	static std::string envBat;
	static std::string cmakePath;
    static std::vector<std::string> cmakeConfigArgs;
	static std::vector<std::string> cmakeBuildArgs;
    static bool clean;

    static int Run();
};
