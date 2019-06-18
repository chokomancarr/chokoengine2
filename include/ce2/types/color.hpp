#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Color : public glm::vec4 {
public:
    Color();
    Color(const float r, const float g, const float b, const float a = 1);
    Color(const float c, const float a = 1);
    Color(const glm::vec4& v);

    static Color black();
    static Color white();
    static Color red();
    static Color green();
    static Color blue();
    static Color yellow();
    static Color cyan();
	static Color gray(float f);
};

CE_END_NAMESPACE