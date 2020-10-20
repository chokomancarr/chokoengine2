#pragma once
#include "chokoplayer.hpp"
#include "detail/pr_defines.hpp"

/* Runtime library for apps generated
 * by ChokoEditor
 */

/* Directory structure
 * project_root/build/
 *   (app_name).exe
 *   assets_index
 *   assets[0~].dat
 *   scene_index
 *   scene[0~].dat
 *   [].dll
 */

CE_BEGIN_MOD_PR_NAMESPACE

class PlayerRelease {
public:
    static void Main();
};

CE_END_MOD_PR_NAMESPACE

#include "detail/pr_includes.hpp"
