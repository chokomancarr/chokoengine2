#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class ShaderBlend {
    Zero, One,
    SrcAlpha,
    DstAlpha,
    OneMinusSrcAlpha,
    OneMinusDstAlpha
};

CE_END_NAMESPACE