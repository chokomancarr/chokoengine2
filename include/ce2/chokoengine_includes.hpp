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
typedef glm::tvec3<int, glm::highp> Int3;
typedef glm::tvec4<int, glm::highp> Int4;

class Color;
class Vec2;
class Vec3;
class Vec4;
class Quat;
class Rect;
class Mat4x4;

class _VertexBuffer;
typedef std::shared_ptr<_VertexBuffer> VertexBuffer;
class _VertexObject;
typedef std::shared_ptr<_VertexObject> VertexObject;

class Math;
class Procedurals;
class Random;
class Subprocess;
class UniqueCallerList;

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
CE_OBJECT(Background)
CE_OBJECT(RenderTarget)

CE_OBJECT(TextureBuffer)

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
CE_OBJECT(CameraEffect)
CE_OBJECT(Light)
CE_OBJECT(MeshRenderer)
CE_OBJECT(MeshModifier)

CE_END_NAMESPACE

#include "types/color.hpp"
#include "types/vec2.hpp"
#include "types/vec3.hpp"
#include "types/vec4.hpp"
#include "types/quat.hpp"
#include "types/rect.hpp"
#include "types/mat4x4.hpp"

#include "utils/math.hpp"
#include "utils/procedural_mesh.hpp"
#include "utils/random.hpp"
#include "utils/subprocess.hpp"
#include "utils/unique_caller.hpp"

#include "core/display.hpp"
#include "core/input.hpp"
#include "core/io.hpp"
#include "core/mvp.hpp"
#include "core/time.hpp"
#include "core/ui.hpp"

#include "object_base.hpp"

#include "asset/asset.hpp"

#include "types/texturebuffer.hpp"
#include "types/depth_texture.hpp"
#include "types/vertexbuffer.hpp"
#include "types/vertexobject.hpp"
#include "types/framebuffer.hpp"

#include "scene/transform.hpp"
#include "scene/scene.hpp"
#include "scene/scene_object.hpp"
#include "scene/component.hpp"