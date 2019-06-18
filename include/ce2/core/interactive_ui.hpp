#pragma once
#include "chokoengine.hpp"
#include "types/ui_button_style.hpp"

CE_BEGIN_NAMESPACE

/* Interactive UI elements.
 * Functions beginning with 'Tr' are not drawn,
 * but behave the same as their non-'Tr' counterparts
 */
class UI::I { CE_CLASS_COMMON
    public:
        static InputMouseStatus Button(const CE_NS Rect& r, const Color& c);

        static InputMouseStatus Button(const CE_NS Rect& r, const UIButtonStyle& s);

        static std::string TextField();

        static float Slider();
};

CE_END_NAMESPACE