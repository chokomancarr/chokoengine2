#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

UniqueCallerList UI::I::textFieldCallers;

void UI::I::PreLoop() {
    textFieldCallers.Preloop();
}

InputMouseStatus UI::I::Button(const CE_NS Rect& r, const UIButtonStyle& s, const std::string& t, const Font& font) {
    const auto ret = ButtonTr(r);
    const auto col = (ret == InputMouseStatus::None) ? s.normal() :
        (((ret == InputMouseStatus::Hover) || (ret == InputMouseStatus::HoverUp)) ? s.hover() : s.pressed());
    Rect(r, col);

    if (!t.empty()) {
        Label(r, t, s.textNormal(), font);
    }

    return ret;
}

InputMouseStatus UI::I::ButtonTr(const CE_NS Rect& r) {
    uint ret = 0;
    if (r.Contains(Input::mousePosition())) {
        ret = 0x10;
        const auto mst = Input::mouseStatus(InputMouseButton::Left);
        if (mst != InputMouseStatus::None) {
            if (r.Contains(Input::mouseDownPosition())) {
                ret |= (uint)mst;
            }
            else {
                ret = 0;
            }
        }
    }
    return (InputMouseStatus)ret;
}

std::string UI::I::TextField(const CE_NS Rect& r, const std::string& s, const UITextFieldStyle& style) {
    bool active = textFieldCallers.Add();

    if (active) {
        if (Button(r, UIButtonStyle(Color::blue())) == InputMouseStatus::HoverUp) {
            textFieldCallers.Clear();
        }
    }
    else {
        if (Button(r, UIButtonStyle(Color::green())) == InputMouseStatus::HoverUp) {
            textFieldCallers.Set();
        }
    }
    return "";
}

float UI::I::Slider(const CE_NS Rect& r, const Vec2& range, float value, const Color& color) {
    Rect(r, color);
    const auto v = SliderTr(r, range, value);
    Rect(CE_NS Rect(r.x() + 1, r.y() + 1, (r.w() - 2) * Math::Clamp(Math::ILerp(range.x, range.y, value), 0.f, 1.f), r.h() - 2), Color::white());
    return v;
}

float UI::I::SliderTr(const CE_NS Rect& r, const Vec2& range, float value) {
    const auto mst = Input::mouseStatus(InputMouseButton::Left);
    if (mst != InputMouseStatus::None) {
        if (r.Contains(Input::mouseDownPosition())) {
            value = Math::Lerp(range.x, range.y, Math::ILerp(r.x(), r.x2(), Input::mousePosition().x));
        }
    }
    return value;
}

CE_END_NAMESPACE