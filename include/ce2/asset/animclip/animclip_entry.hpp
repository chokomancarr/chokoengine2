#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimClip::Entry { CE_OBJECT_COMMON
public:
	std::string signature;
	Curve<Vec3> positions;
	Curve<Quat> rotations;
	Curve<Vec3> scales;
};

CE_END_NAMESPACE