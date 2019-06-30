#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class LightType {
    /* 360-degree light from a center point
     */
    Point,
    /* Fixed-degree light from a center point
     */
    Spot,
    /* Uni-directional light from infinite source
     */
    Directional
};

CE_END_NAMESPACE