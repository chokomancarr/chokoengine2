#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Random { CE_CLASS_COMMON

    static float RadicalInverse(uint bits);
public:
    static std::vector<Vec2> Hammersley(uint n);
};

CE_END_NAMESPACE