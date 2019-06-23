#pragma once
#include "chokoengine.hpp"

#include "backend/chokoengine_backend_base.hpp"

#include "core/debug.hpp"

#include "ext/ptrext.hpp"
#include "ext/glext.hpp"
#include "ext/strext.hpp"

CE_BEGIN_NAMESPACE

/* types
 */
typedef glm::tvec2<int, glm::highp> Int2;
typedef glm::vec2 Vec2;
typedef glm::tvec3<int, glm::highp> Int3;
typedef glm::vec3 Vec3;
typedef glm::tvec4<int, glm::highp> Int4;
typedef glm::vec4 Vec4;

class Color;
class Quat;
class Rect;
class Mat4x4;

CE_END_NAMESPACE

#include "types/color.hpp"
#include "types/quat.hpp"
#include "types/rect.hpp"
#include "types/mat4x4.hpp"
#include "types/vertexbuffer.hpp"
#include "types/vertexobject.hpp"
#include "types/texturebuffer.hpp"

#include "utils/math.hpp"

CE_BEGIN_NAMESPACE

class Object;
typedef std::shared_ptr<Object> pObject;

/* assets
 */
CE_OBJECT(Asset)
CE_OBJECT(Font)
CE_OBJECT(Material)
CE_OBJECT(Mesh)
CE_OBJECT(Shader)
CE_OBJECT(Texture)
CE_OBJECT(RenderTarget)

/* scene
 */
class Transform;
typedef Transform* pTransform;
CE_OBJECT(Scene)
CE_OBJECT(SceneObject)

/* components
 */
CE_OBJECT(Component)
CE_OBJECT(Camera)
CE_OBJECT(MeshRenderer)
CE_OBJECT(MeshModifier)

CE_END_NAMESPACE

#include "core/display.hpp"
#include "core/input.hpp"
#include "core/io.hpp"
#include "core/mvp.hpp"
#include "core/time.hpp"
#include "core/ui.hpp"

#include "object_base.hpp"

#include "asset/asset.hpp"

#include "scene/transform.hpp"
#include "scene/scene.hpp"
#include "scene/scene_object.hpp"
#include "scene/component.hpp"