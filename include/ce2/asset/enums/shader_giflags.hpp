#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

typedef uint SHADER_GI_FLAGS;

const uint SHADER_GI_EMIT_COLOR = 1 << 0;

const uint SHADER_GI_EMIT_TEXTURE = 1 << 1;

CE_END_NAMESPACE