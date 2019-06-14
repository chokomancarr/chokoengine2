#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Quat : public glm::quat {
public:
    Quat();
    Quat(float w, Vec3 v);
    Quat(float w, float x, float y, float z);

    static Quat identity();
};

CE_END_NAMESPACE