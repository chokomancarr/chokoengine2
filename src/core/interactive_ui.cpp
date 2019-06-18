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
        if (mst != InputMouseStatus::None) {
            if (r.Contains(Input::mouseDownPosition())) {
                ret |= (uint)mst;
            }
        }
    }
    
    const auto col = (!ret) ? s.normal() :
        ((ret == 0x10) ? s.hover() : s.pressed());
    UI::Rect(r, col);
}

std::string UI::I::TextField() {

}

float UI::I::Slider() {

}

CE_END_NAMESPACE