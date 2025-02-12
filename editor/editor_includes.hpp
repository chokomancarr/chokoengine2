#pragma once

CE_BEGIN_ED_NAMESPACE

class EPaths;
class EWindow;

CE_OBJECT(VShader)

CE_END_ED_NAMESPACE

#include "editor_paths.hpp"

#include "asset/ext/visual_shader.hpp"

#include "builder/builder.hpp"

#include "preview/preview.hpp"

#include "scripting/scripting.hpp"

#include "res/icons.hpp"
#include "res/images.hpp"

#include "hooks/callback_manager.hpp"

#include "ui/colors.hpp"
#include "ui/layout/ui_layout.hpp"

#include "asset/asset_list.hpp"
#include "asset/asset_loader.hpp"
#include "asset/asset_manager.hpp"
#include "asset/asset_writer.hpp"

#include "player/player.hpp"

#include "scene/scenemanager.hpp"
#include "scene/sceneinfo.hpp"

#include "ui/dragdrop.hpp"
#include "ui/dropdown_menu.hpp"
#include "ui/windowmanager.hpp"
#include "ui/overlaymanager.hpp"

#include "utils/debug2.hpp"

#include "serial/serializer.hpp"

#include "taskrunners/tasklist.hpp"