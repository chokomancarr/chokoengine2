#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class InputMouseStatus {
    None = 0x0,
    Down, Hold, Up,
    //Hover flag (0x10) is appended when mouse is above a button
    Hover = 0x10,
    HoverDown, HoverHold, HoverUp
};

CE_END_NAMESPACE
