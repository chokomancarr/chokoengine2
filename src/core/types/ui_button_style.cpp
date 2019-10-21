#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

UIButtonStyle::UIButtonStyle(const Color& n, const Color& t)
        : UIButtonStyle(n, Math::Lerp(n, Color::white(), 0.5f), Math::Lerp(n, Color::black(), 0.5f), t, t, t) {}

UIButtonStyle::UIButtonStyle(const Color& n, const Color& h, const Color& p, const Color& tn, const Color& th, const Color& tp)
        : _normal(n), _hover(h), _pressed(p), _textNormal(tn), _textHover(th), _textPressed(tp) {}

CE_END_NAMESPACE