#pragma once
#include "chokoengine.hpp"
#include "bd_defines.hpp"

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
    static std::vector<std::string> cmakeConfigArgs;
    static bool clean;

    static int Run();
};

#include "bd_includes.hpp"
