#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

InputMouseStatus UI::I::Button(const CE_NS Rect& r, const Color& c) {
    return Button(r, UIButtonStyle(c,
        Math::Lerp(c, Color::white(), 0.5f),
        Math::Lerp(c, Color::black(), 0.5f)));
}

InputMouseStatus UI::I::Button(const CE_NS Rect& r, const UIButtonStyle& s) {
    uint ret = 0;
    if (r.Contains(Input::mousePosition())) {
        ret = 0x10;
        const auto mst = Input::mouseStatus(InputMouseButton::Left);
        if ((mst == InputMouseStatus::Down)
                || (mst == InputMouseStatus::Hold)) {
            if (r.Contains(Input::mouseDownPosition())) {
                ret |= (uint)mst;
            }
        }
    }
    std::cout << std::hex << ret << std::endl;
    const auto col = (!ret) ? s.normal() :
        ((ret == 0x10) ? s.hover() : s.pressed());
    UI::Rect(r, col);

    return (InputMouseStatus)ret;
}

std::string UI::I::TextField() {
    return "";
}

float UI::I::Slider() {
    return 0;
}

CE_END_NAMESPACE