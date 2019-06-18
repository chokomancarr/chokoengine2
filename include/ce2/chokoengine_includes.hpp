#pragma once
#include "chokoengine.hpp"

#include "core/debug.hpp"

#include "ext/ptrext.hpp"
#include "ext/glext.hpp"
#include "ext/strext.hpp"

#include "types/color.hpp"
#include "types/quat.hpp"
#include "types/rect.hpp"
#include "types/mat4x4.hpp"
#include "types/vertexbuffer.hpp"
#include "types/vertexobject.hpp"
#include "types/texturebuffer.hpp"

#include "utils/math.hpp"

CE_BEGIN_NAMESPACE

/* assets
 */
CE_OBJECT(Font)
CE_OBJECT(Shader)
CE_OBJECT(Texture)

/* scene
 */
class Object;
class Transform;
typedef Transform* pTransform;
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

#include "core/display.hpp"
#include "core/input.hpp"
#include "core/io.hpp"
#include "core/mvp.hpp"
#include "core/ui.hpp"

#include "scene/object_base.hpp"
#include "scene/transform.hpp"
#include "scene/scene.hpp"
#include "scene/sceneobject.hpp"
#include "scene/component.hpp"