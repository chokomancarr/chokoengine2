#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Vec2 Input::_mousePositionOld;
Vec2 Input::_mousePosition;
Vec2 Input::_mouseDownPosition;

std::array<bool, 5> Input::_mouseButtonStatesOld;
std::array<bool, 5> Input::_mouseButtonStates;

bool Input::Init() {
    glfwSetCursorPosCallback(Display::_window, _OnCursorMove);
    glfwSetMouseButtonCallback(Display::_window, _OnMouseClick);
    return true;
}

void Input::PreLoop() {
    if (mouseStatus(InputMouseButton::Left) == InputMouseStatus::Down) {
        _mouseDownPosition = _mousePosition;
    }
}

void Input::PostLoop() {
    _mousePositionOld = _mousePosition;
    _mouseButtonStatesOld = _mouseButtonStates;
}

void Input::_OnCursorMove(GLFWwindow*, double x, double y) {
    _mousePosition = Vec2((float)x, (float)y);
}

void Input::_OnMouseClick(GLFWwindow*, int btn, int vl, int mod) {
    if (btn < 5)
        _mouseButtonStates[btn] = !!vl;
}

Vec2 Input::mouseDelta() {
    return _mousePosition - _mousePositionOld;
}

InputMouseStatus Input::mouseStatus(InputMouseButton b) {
    const int i = (int)b;
    if (_mouseButtonStates[i]) {
        return _mouseButtonStatesOld[i] ? 
            InputMouseStatus::Hold : InputMouseStatus::Down;
    }
    else {
        return _mouseButtonStatesOld[i] ? 
            InputMouseStatus::Up : InputMouseStatus::None;
    }
}

bool Input::KeyDown(InputKey c) {
    return false;
}

bool Input::KeyHold(InputKey c) {
    return false;
}

bool Input::KeyUp(InputKey c) {
    return false;
}

CE_END_NAMESPACE