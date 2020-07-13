#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

CE_BEGIN_NAMESPACE

#define IMPL(src, dst)\
inline dst glm_cast(const glm::src& val) {\
	return *(dst*)&val;\
}

IMPL(vec2, Vec2)
IMPL(vec3, Vec3)
IMPL(vec4, Vec4)
IMPL(mat3, Mat3x3)
IMPL(mat4, Mat4x4)
IMPL(quat, Quat)


template <typename T>
float* fptr(const T& v) {
	return (float*)&v;
}

CE_END_NAMESPACE