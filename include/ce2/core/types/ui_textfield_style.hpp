#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class UITextFieldStyle {
    Color _normal, _hover, _pressed, _editing;
    Color _textNormal, _textHover, _textPressed, _textEditing;
    Color _highlight, _textHighlight;
public:
    UITextFieldStyle(const Color& n);
    UITextFieldStyle(const Color& n, const Color& h, const Color& p, const Color& e);

    CE_GET_SET_MEMBER(normal);
    CE_GET_SET_MEMBER(hover);
    CE_GET_SET_MEMBER(pressed);
    CE_GET_SET_MEMBER(editing);
    CE_GET_SET_MEMBER(textNormal);
    CE_GET_SET_MEMBER(textHover);
    CE_GET_SET_MEMBER(textPressed);
    CE_GET_SET_MEMBER(textEditing);
    CE_GET_SET_MEMBER(highlight);
    CE_GET_SET_MEMBER(textHighlight);
};

CE_END_NAMESPACE