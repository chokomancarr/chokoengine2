#pragma once
#include "chokoengine.hpp"

#include "ext/ptrext.hpp"
#include "ext/glext.hpp"
#include "ext/strext.hpp"

#include "types/color.hpp"
#include "types/rect.hpp"

CE_BEGIN_NAMESPACE

/* assets
 */
CE_OBJECT(Font)
CE_OBJECT(Shader)
CE_OBJECT(Texture)

/* scene
 */
CE_OBJECT(Scene)
CE_OBJECT(SceneObject)
CE_OBJECT(Component)

/* components
 */
CE_OBJECT(Camera)

CE_END_NAMESPACE

#include "asset/font.hpp"
#include "asset/shader.hpp"
#include "asset/texture.hpp"

#include "core/debug.hpp"
#include "core/display.hpp"
#include "core/io.hpp"
#include "core/mvp.hpp"
#include "core/ui.hpp"

#include "scene/scene.hpp"
#include "scene/sceneobject.hpp"
#include "scene/component.hpp"