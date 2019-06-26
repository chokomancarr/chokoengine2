#pragma once
#include "chokoengine.hpp"
#include "types/ui_button_style.hpp"

CE_BEGIN_NAMESPACE

/* Interactive UI elements.
 * Functions ending with 'Tr' are not drawn,
 * but behave the same as their non-'Tr' counterparts
 */
class UI::I { CE_CLASS_COMMON
    static UniqueCallerList textFieldCallers;

    static void PreLoop();

public:

    /* Draws a clickable button with dimensions \p r.
     * If a string is provided, a label is drawn inside the button.
     */
    static InputMouseStatus Button(const CE_NS Rect& r, const UIButtonStyle& s, const std::string& t = "", const Font& font = _defaultFont);

    static InputMouseStatus ButtonTr(const CE_NS Rect& r);

    static std::string TextField(const CE_NS Rect& r);

    static float Slider(const CE_NS Rect& r, const Vec2& range, float value, const Color& color);

    static float SliderTr(const CE_NS Rect& r, const Vec2& range, float value);
};

CE_END_NAMESPACE