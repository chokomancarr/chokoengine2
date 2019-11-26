#pragma once
#include "chokoengine.hpp"

#ifdef PLATFORM_WIN
#include "minimalwin.h"
#else
#include <unistd.h>
#endif

#ifdef PLATFORM_MAC
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

CE_BEGIN_NAMESPACE

/* type extensions
 */
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

typedef glm::tvec2<int, glm::highp> Int2;
typedef glm::tvec3<int, glm::highp> Int3;
typedef glm::tvec4<int, glm::highp> Int4;

CE_END_NAMESPACE

/* engine headers
*/
#include "backend/chokoengine_backend_base.hpp"

#include "core/debug.hpp"

#include "ext/ptrext.hpp"
#include "ext/glext.hpp"
#include "ext/strext.hpp"

CE_BEGIN_NAMESPACE

/* class declarations
 */
class Color;
class Vec2;
class Vec3;
class Vec4;
class Quat;
class Rect;
class Mat4x4;

class _VertexBuffer;
typedef std::shared_ptr<_VertexBuffer> VertexBuffer;
class _VertexArray;
typedef std::shared_ptr<_VertexArray> VertexArray;

class Math;
class Procedurals;
class Random;
class Subprocess;
class UniqueCallerList;

CE_OBJECT(Object)

/* assets
 */
CE_OBJECT(AnimClip)
CE_OBJECT(AnimGraph)
CE_OBJECT(Asset)
CE_OBJECT(Armature)
CE_OBJECT(CubeMap)
CE_OBJECT(Font)
CE_OBJECT(Material)
CE_OBJECT(Mesh)
CE_OBJECT(Prefab)
CE_OBJECT(Shader)
CE_OBJECT(Texture)
CE_OBJECT(Texture3)
CE_OBJECT(Background)
CE_OBJECT(RenderTarget)
CE_OBJECT(RenderTarget3)

CE_OBJECT(TextureBuffer)

/* scene
 */
CE_OBJECT(Scene)
CE_OBJECT(SceneObject)
class Transform;
typedef Transform* pTransform;

/* components
 */
CE_OBJECT(Component)
CE_OBJECT(Animator)
CE_OBJECT(Camera)
CE_OBJECT(CameraEffect)
CE_OBJECT(Light)
CE_OBJECT(LightProbe)
CE_OBJECT(MeshRenderer)
CE_OBJECT(MeshModifier)
CE_OBJECT(MeshShapeModifier)
CE_OBJECT(MeshSkinModifier)
CE_OBJECT(MeshClothModifier)
CE_OBJECT(Rig)
CE_OBJECT(Script)

CE_END_NAMESPACE

#include "types/color.hpp"
#include "types/vec2.hpp"
#include "types/vec3.hpp"
#include "types/vec4.hpp"
#include "types/quat.hpp"
#include "types/rect.hpp"
#include "types/mat4x4.hpp"

#include "utils/math.hpp"
#include "utils/curve.hpp"
#include "utils/procedural_mesh.hpp"
#include "utils/random.hpp"
#include "utils/subprocess.hpp"
#include "utils/transform_feedback.hpp"
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
#include "types/depth_cubemap.hpp"
#include "types/vertexbuffer.hpp"
#include "types/vertexarray.hpp"
#include "types/framebuffer.hpp"
#include "types/cube_framebuffer.hpp"

#include "scene/transform.hpp"
#include "scene/scene.hpp"
#include "scene/scene_object.hpp"
#include "scene/component.hpp"
