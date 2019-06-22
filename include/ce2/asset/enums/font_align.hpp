#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class FontAlign {
	TopLeft = 0x00, TopCenter, TopRight,
	MiddleLeft = 0x10, MiddleCenter, MiddleRight,
	BottomLeft = 0x20, BottomCenter, BottomRight
};

CE_END_NAMESPACE