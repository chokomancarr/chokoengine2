#pragma once
#include "defines.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
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

CE_BEGIN_NAMESPACE

class ChokoLait;

class ChokoEngine {
public:
	typedef uint64_t objectid;

	static bool Init();
	static void Cleanup();
};

CE_END_NAMESPACE

#include "chokoengine_includes.hpp"