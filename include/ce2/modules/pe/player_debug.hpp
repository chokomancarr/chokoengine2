#pragma once
#include "chokoplayer.hpp"
#include "detail/pe_defines.hpp"

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

CE_BEGIN_MOD_PE_NAMESPACE

class PlayerDebug {
public:
    static std::string projectPath;

    static void Init();

    static void Main();
};

CE_END_MOD_PE_NAMESPACE

#include "detail/pe_includes.hpp"
