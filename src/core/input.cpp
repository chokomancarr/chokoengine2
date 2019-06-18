#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Vec2 Input::_mousePosition;
Vec2 Input::_mouseDownPosition;

InputMouseStatus Input::mouseStatus(InputMouseButton b) {
    return InputMouseStatus::None;
}

CE_END_NAMESPACE