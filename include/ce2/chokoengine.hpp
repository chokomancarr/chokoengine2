#pragma once
#include "defines.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>

#ifdef PLATFORM_WIN
#include "minimalwin.h"
#else
#include <unistd.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

typedef glm::tvec2<int, glm::highp> Int2;
typedef glm::vec2 Vec2;
typedef glm::tvec2<int, glm::highp> Int3;
typedef glm::vec3 Vec3;
typedef glm::tvec3<int, glm::highp> Int4;
typedef glm::vec4 Vec4;
typedef glm::mat4 Mat4x4;

class ChokoLait;

CE_BEGIN_NAMESPACE

class Engine {
public:
	static bool Init();
	static void Cleanup();
};

CE_END_NAMESPACE

#include "chokoengine_includes.hpp"