#pragma once
#include "chokoengine.hpp"

#include "ext/ptrext.hpp"
#include "ext/glext.hpp"
#include "ext/refcnt.hpp"
#include "ext/strext.hpp"

BEGIN_CE_NAMESPACE

/* assets
 */
CE_OBJECT(Font)
CE_OBJECT(Shader)

/* scene
 */
CE_OBJECT(Scene)
CE_OBJECT(SceneObject)
CE_OBJECT(Component)

/* components
 */
CE_OBJECT(Camera)

END_CE_NAMESPACE

#include "asset/font.hpp"
#include "asset/shader.hpp"

#include "core/debug.hpp"
#include "core/display.hpp"
#include "core/io.hpp"
#include "core/mvp.hpp"

#include "scene/scene.hpp"
#include "scene/sceneobject.hpp"
#include "scene/component.hpp"