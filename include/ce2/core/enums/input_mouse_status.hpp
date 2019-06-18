#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class InputMouseStatus {
    None = 0x0,
    Down = 0x1, Hold, Up,
    //Hover flag (0x10) is appended when mouse is above a button
    HoverDown = 0x11, HoverHold, HoverUp
};

CE_END_NAMESPACE
