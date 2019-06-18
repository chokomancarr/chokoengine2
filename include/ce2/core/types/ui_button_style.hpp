#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class UIButtonStyle {
    Color _normal, _hover, _pressed;
public:
    UIButtonStyle(const Color& n, const Color& h, const Color& p);

    CE_GET_SET_MEMBER(normal);
    CE_GET_SET_MEMBER(hover);
    CE_GET_SET_MEMBER(pressed);
};

CE_END_NAMESPACE