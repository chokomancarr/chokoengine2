#include "turner.hpp"

using namespace ChokoEngine;

void _Turner::OnUpdate() {
    if (obj != nullptr) {
        obj->transform()->Rotate(Vec3(0, Time::delta() * rate1, 0));
    }
    Debug::Message("Turner", "update!");
}