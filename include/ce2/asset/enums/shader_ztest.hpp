#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class ShaderZTest {
    Never, Always,
    Less,
    LessEqual,
    Equal,
    GreaterEqual,
    Greater
};

CE_END_NAMESPACE