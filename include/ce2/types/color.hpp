#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Color { CE_OBJECT_COMMON
    float _r, _g, _b, _a;

public:
    static Color black();
    static Color white();
    static Color red();
    static Color green();
    static Color blue();
    static Color yellow();
    static Color cyan();

    Color();
    Color(const float r, const float g, const float b, const float a = 1);
    Color(const float c, const float a = 1);

    CE_GET_SET_MEMBER(r);
    CE_GET_SET_MEMBER(g);
    CE_GET_SET_MEMBER(b);
    CE_GET_SET_MEMBER(a);
};

CE_END_NAMESPACE