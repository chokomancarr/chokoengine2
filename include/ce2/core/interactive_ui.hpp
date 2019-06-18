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

    /* Draws a clickable button with dimensions r.
     * If a string is provided, a label is drawn inside the button.
     */
    static InputMouseStatus Button(const CE_NS Rect& r, const UIButtonStyle& s, const std::string& t = "", const Font& font = _defaultFont);

    static InputMouseStatus TrButton(const CE_NS Rect& r);

    static std::string TextField();

    static float Slider();
};

CE_END_NAMESPACE