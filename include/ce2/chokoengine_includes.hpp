#pragma once
#include "chokoengine.hpp"

#include <algorithm>
#include <array>
#include <assert.h> 
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

CE_BEGIN_NAMESPACE

/* type extensions
 */
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

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
class CRValue;
class Vec2;
class Vec3;
class Vec4;
class Int2;
class Int3;
class Int4;
class FCurve;
class Gradient;
class Quat;
class Rect;
class Mat3x3;
class Mat4x4;

class _VertexBuffer;
typedef std::shared_ptr<_VertexBuffer> VertexBuffer;
class _VertexArray;
typedef std::shared_ptr<_VertexArray> VertexArray;

class Cursor;

class PrefabManager;

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
#include "scene/comp/detail/compdefsall.inl"
COMP_DEFS_ALL(CE_OBJECT)
#include "scene/comp/detail/compdefsall_undef.inl"
CE_OBJECT(CameraEffect)
CE_OBJECT(MeshModifier)
CE_OBJECT(MeshShapeModifier)
CE_OBJECT(MeshSkinModifier)
CE_OBJECT(MeshClothModifier)

CE_END_NAMESPACE

#include "types/color.hpp"
#include "types/crvalue.hpp"
#include "types/vec2.hpp"
#include "types/vec3.hpp"
#include "types/vec4.hpp"
#include "types/int2.hpp"
#include "types/int3.hpp"
#include "types/int4.hpp"
#include "types/fcurve.hpp"
#include "types/gradient.hpp"
#include "types/quat.hpp"
#include "types/rect.hpp"
#include "types/mat3x3.hpp"
#include "types/mat4x4.hpp"

#include "parsers/json.hpp"
#include "parsers/mesh.hpp"

#include "physics/physics.hpp"

#include "utils/cursor.hpp"
#include "utils/math.hpp"
#include "utils/curve.hpp"
#include "utils/procedural_mesh.hpp"
#include "utils/random.hpp"
#include "utils/shared_mem.hpp"
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
#include "asset/prefab_manager.hpp"

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
