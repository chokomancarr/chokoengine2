#pragma once

CE_BEGIN_ED_NAMESPACE

class EPaths;
class EWindow;

CE_OBJECT(VShader)

CE_END_ED_NAMESPACE

#include "editor_paths.hpp"

#include "asset/ext/visual_shader.hpp"

#include "preview/preview.hpp"

#include "scripting/scripting.hpp"

#include "res/icons.hpp"
#include "res/images.hpp"

#include "hooks/callback.hpp"

#include "ui/colors.hpp"
#include "ui/layout/ui_layout.hpp"

#include "asset/asset_list.hpp"
#include "asset/asset_loader.hpp"
#include "asset/prefab_manager.hpp"

#include "scene/sceneinfo.hpp"

#include "ui/dropdown_menu.hpp"
#include "ui/windowmanager.hpp"
#include "ui/overlaymanager.hpp"

#include "serial/serializer.hpp"
#include "serial/serialized_object.hpp"