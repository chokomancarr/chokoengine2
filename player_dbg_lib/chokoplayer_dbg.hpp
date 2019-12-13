#pragma once
#include "chokoplayer.hpp"

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

CE_BEGIN_PL_NAMESPACE

class ChokoPlayerD {
public:
    static std::string projectPath;

    static void Init();

    static void Main();
};

CE_END_PL_NAMESPACE

#include "player_dbg_includes.hpp"
