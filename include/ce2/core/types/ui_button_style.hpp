#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class UIButtonStyle {
    Color _normal, _hover, _pressed;
    Color _textNormal, _textHover, _textPressed;
public:
    UIButtonStyle(const Color& n, const Color& t = Color::white());
    UIButtonStyle(const Color& n, const Color& h, const Color& p, const Color& tn = Color::white(), const Color& th = Color::white(), const Color& tp = Color::white());

    CE_GET_SET_MEMBER(normal);
    CE_GET_SET_MEMBER(hover);
    CE_GET_SET_MEMBER(pressed);
    CE_GET_SET_MEMBER(textNormal);
    CE_GET_SET_MEMBER(textHover);
    CE_GET_SET_MEMBER(textPressed);
};

CE_END_NAMESPACE