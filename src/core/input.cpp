#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Vec2 Input::_mousePositionOld;
Vec2 Input::_mousePosition;
Vec2 Input::_mouseDownPosition;

Vec2 Input::_mouseScroll;

std::array<bool, 5> Input::_mouseButtonStatesOld = {};
std::array<bool, 5> Input::_mouseButtonStates = {};

std::array<bool, 350> Input::_keyStatesOld = {};
std::array<bool, 350> Input::_keyStates = {};

std::string Input::_inputString;
std::u32string Input::_inputUnicodeString;

bool Input::Init() {
    glfwSetCursorPosCallback(Display::_window, _OnCursorMove);
    glfwSetMouseButtonCallback(Display::_window, _OnMouseClick);
	glfwSetScrollCallback(Display::_window, _OnMouseScroll);
    glfwSetInputMode(Display::_window, GLFW_STICKY_KEYS, true);
    glfwSetKeyCallback(Display::_window, _OnKeyPress);
    glfwSetCharCallback(Display::_window, _OnCharInput);
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
    _keyStatesOld = _keyStates;
	_mouseScroll = 0;
    _inputString.clear();
    _inputUnicodeString.clear();
}

void Input::_OnCursorMove(GLFWwindow*, double x, double y) {
    _mousePosition = Vec2((float)x, (float)y);
}

void Input::_OnMouseClick(GLFWwindow*, int btn, int vl, int mod) {
    if (btn < 5)
        _mouseButtonStates[btn] = !!vl;
}

void Input::_OnMouseScroll(GLFWwindow*, double x, double y) {
	_mouseScroll = Vec2((float)x, (float)y);
}

void Input::_OnKeyPress(GLFWwindow*, int key, int, int act, int) {
    if (key < 350) _keyStates[key] = (act == GLFW_PRESS);
}

void Input::_OnCharInput(GLFWwindow*, uint codepoint) {
    if (codepoint <= 0x7f) {
        _inputString.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7ff) {
        _inputString.push_back(static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        _inputString.push_back(static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff) {
        _inputString.push_back(static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        _inputString.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        _inputString.push_back(static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else {
        _inputString.push_back(static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        _inputString.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        _inputString.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        _inputString.push_back(static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    _inputUnicodeString.push_back(codepoint);
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
    return _keyStates[(uint)c] && !_keyStatesOld[(int)c];
}

bool Input::KeyHold(InputKey c) {
    return _keyStates[(uint)c] && _keyStatesOld[(int)c];
}

bool Input::KeyUp(InputKey c) {
    return !_keyStates[(uint)c] && _keyStatesOld[(int)c];
}

CE_END_NAMESPACE