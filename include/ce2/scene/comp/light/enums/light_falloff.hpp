#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class LightFalloff {
    Constant,
    Linear,
    Squared
};

CE_END_NAMESPACE

#include "light_falloff_str.hpp"