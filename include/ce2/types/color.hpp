#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

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

    GET_SET_MEMBER_FUNC(float, r);
    GET_SET_MEMBER_FUNC(float, g);
    GET_SET_MEMBER_FUNC(float, b);
    GET_SET_MEMBER_FUNC(float, a);
};

END_CE_NAMESPACE