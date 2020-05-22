#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class LightType {
    Point, /* 360-degree light from a center point */
    Spot, /* Fixed-degree light from a center point */
    Directional, /* Uni-directional light from infinite source */
};

CE_END_NAMESPACE

#include "light_type_str.hpp"