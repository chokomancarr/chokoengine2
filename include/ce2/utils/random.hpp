#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace Random {

	float Value();

	float Range(float min, float max);

    std::vector<Vec2> Hammersley(uint n);
}

CE_END_NAMESPACE