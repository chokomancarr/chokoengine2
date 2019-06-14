#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Quat::Quat() : glm::quat() {}

Quat::Quat(float w, Vec3 v) : glm::quat(w, v) {}

Quat::Quat(float w, float x, float y, float z) : glm::quat(w, x, y, z) {}

Quat Quat::identity() {
    return Quat(1, 0, 0, 0);
}

CE_END_NAMESPACE