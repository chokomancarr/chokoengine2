#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class LightFalloff {
    Constant,
    Linear,
    Inverse,
    InverseSquared
};

CE_END_NAMESPACE