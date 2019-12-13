#pragma once
#include "chokoplayer_dbg.hpp"

#define CE_PL_PATH_ASSET (ChokoPlayerD::projectPath + "asset/")
#define CE_PL_PATH_CONFIG (ChokoPlayerD::projectPath + "config/")
#define CE_PL_PATH_RES (ChokoPlayerD::projectPath + "resources/")
#define CE_PL_PATH_SYSTEM (ChokoPlayerD::projectPath + "system/")

CE_BEGIN_PL_NAMESPACE

CE_PL_OBJ(AssetLoader)

CE_END_PL_NAMESPACE

#include "asset/asset_loader.hpp"
