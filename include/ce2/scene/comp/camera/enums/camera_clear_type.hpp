#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class CameraClearType {
    None,
    Depth,
    Color,
    ColorAndDepth
};

CE_END_NAMESPACE

#include "camera_clear_type_str.hpp"