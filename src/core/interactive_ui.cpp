#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

InputMouseStatus UI::I::Button(const CE_NS Rect& r, const UIButtonStyle& s, const std::string& t, const Font& font) {
    const auto ret = TrButton(r);
    const auto col = (ret == InputMouseStatus::None) ? s.normal() :
        ((ret == InputMouseStatus::Hover) ? s.hover() : s.pressed());
    Rect(r, col);

    if (!t.empty()) {
        Label(r, t, s.textNormal(), font);
    }

    return ret;
}

InputMouseStatus UI::I::TrButton(const CE_NS Rect& r) {
    uint ret = 0;
    if (r.Contains(Input::mousePosition())) {
        ret = 0x10;
        const auto mst = Input::mouseStatus(InputMouseButton::Left);
        if ((mst == InputMouseStatus::Down)
                || (mst == InputMouseStatus::Hold)) {
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

std::string UI::I::TextField() {
    return "";
}

float UI::I::Slider() {
    return 0;
}

CE_END_NAMESPACE