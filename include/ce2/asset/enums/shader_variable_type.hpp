#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class ShaderVariableType {
	Unknown,
	Float,
	Int,
	Vec2,
	Vec3,
	Vec4,
	Matrix,
	Texture
};

CE_END_NAMESPACE