#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Input::State::State() : mousePositionOld(0), mousePosition(0), mouseDownPosition(0),
        mouseScroll(0), mouseButtonStatesOld(), mouseButtonStates(), keyStatesOld(),
        keyStates(), inputString(), inputUnicodeString() {}


Input::State Input::_state = Input::State();

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
        _state.mouseDownPosition = _state.mousePosition;
    }
}

void Input::PostLoop() {
    _state.mousePositionOld = _state.mousePosition;
    _state.mouseButtonStatesOld = _state.mouseButtonStates;
    _state.keyStatesOld = _state.keyStates;
	_state.mouseScroll = 0;
    _state.inputString.clear();
    _state.inputUnicodeString.clear();
}

void Input::_OnCursorMove(GLFWwindow*, double x, double y) {
    _state.mousePosition = Vec2((float)x, (float)y);
}

void Input::_OnMouseClick(GLFWwindow*, int btn, int vl, int mod) {
    if (btn < 5)
        _state.mouseButtonStates[btn] = !!vl;
}

void Input::_OnMouseScroll(GLFWwindow*, double x, double y) {
	_state.mouseScroll = Vec2((float)x, (float)y);
}

void Input::_OnKeyPress(GLFWwindow*, int key, int, int act, int) {
    if (key < 350) _state.keyStates[key] = (act == GLFW_PRESS);
}

void Input::_OnCharInput(GLFWwindow*, uint codepoint) {
    if (codepoint <= 0x7f) {
        _state.inputString.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7ff) {
        _state.inputString.push_back(static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        _state.inputString.push_back(static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff) {
        _state.inputString.push_back(static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        _state.inputString.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        _state.inputString.push_back(static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else {
        _state.inputString.push_back(static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        _state.inputString.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        _state.inputString.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        _state.inputString.push_back(static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    _state.inputUnicodeString.push_back(codepoint);
}

void Input::Clear() {
    _state = State();
}

#define CE_I_GETF(t, nm) const t& Input::nm() {\
    return _state.nm;\
}

CE_I_GETF(Vec2, mousePosition);

CE_I_GETF(Vec2, mouseDownPosition);

Vec2 Input::mouseDelta() {
    return _state.mousePosition - _state.mousePositionOld;
}

CE_I_GETF(Vec2, mouseScroll);

CE_I_GETF(std::string, inputString);

CE_I_GETF(std::u32string, inputUnicodeString);

InputMouseStatus Input::mouseStatus(InputMouseButton b) {
    const int i = (int)b;
    if (_state.mouseButtonStates[i]) {
        return _state.mouseButtonStatesOld[i] ? 
            InputMouseStatus::Hold : InputMouseStatus::Down;
    }
    else {
        return _state.mouseButtonStatesOld[i] ? 
            InputMouseStatus::Up : InputMouseStatus::None;
    }
}

bool Input::KeyDown(InputKey c) {
    return _state.keyStates[(uint)c] && !_state.keyStatesOld[(int)c];
}

bool Input::KeyHold(InputKey c) {
    return _state.keyStates[(uint)c] && _state.keyStatesOld[(int)c];
}

bool Input::KeyUp(InputKey c) {
    return !_state.keyStates[(uint)c] && _state.keyStatesOld[(int)c];
}

CE_END_NAMESPACE