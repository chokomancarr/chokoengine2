#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class ColliderType {
	InfPlane,
	Plane,
	Sphere,
    Cube,
    Capsule,
    Mesh
};

CE_END_NAMESPACE

#include "collider_type_str.hpp"