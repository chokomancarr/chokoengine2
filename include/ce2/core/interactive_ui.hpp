#pragma once
#include "chokoengine.hpp"
#include "types/ui_button_style.hpp"
#include "types/ui_textfield_style.hpp"

CE_BEGIN_NAMESPACE

/* Interactive UI elements
 * Functions ending with 'Tr' are not drawn,
 * but behave the same as their non-'Tr' counterparts
 */
class UI::I { CE_CLASS_COMMON
    static UniqueCallerList textFieldCallers;

    static struct _TextFieldInfo {
        bool editing;
        uint cursor, cursor2;
        std::string buffer;
        std::u32string ubuffer;
        float time;
    } _textFieldInfo;

    static void PreLoop();

public:
    /* Draws a clickable button with dimensions \p r
     * The optional string is drawn inside the button
     */
    static InputMouseStatus Button(const CE_NS Rect& r, const UIButtonStyle& s, const std::string& t = "", const Font& font = _defaultFont);

	/* Draws a clickable button with dimensions \p r
	 * using texture \p t as the button
	 */
	static InputMouseStatus Button(const CE_NS Rect& r, const UIButtonStyle& s, const CE_NS Texture& t, const Color& c = Color::white());

    /* A clickable button with dimensions \p r
     */
    static InputMouseStatus ButtonTr(const CE_NS Rect& r);

    /* Draws a text box with editable contents
     */
    static std::string TextField(const CE_NS Rect& r, const std::string& s, const UITextFieldStyle& style);

    /* Draws a horizontal slider with dimensions \p r
     * The value is clamped between \p range
     */
    static float Slider(const CE_NS Rect& r, const Vec2& range, float value, const Color& color);

    /* A horizontal slider with dimensions \p r
     * The value is clamped between \p range
     */
    static float SliderTr(const CE_NS Rect& r, const Vec2& range, float value);
};

CE_END_NAMESPACE