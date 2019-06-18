#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

UIButtonStyle::UIButtonStyle(const Color& n)
        : _normal(n), _hover(Math::Lerp(n, Color::white(), 0.5f)),
        _pressed(Math::Lerp(n, Color::black(), 0.5f)) {}

UIButtonStyle::UIButtonStyle(const Color& n, const Color& h, const Color& p)
        : _normal(n), _hover(h), _pressed(p), _textNormal(Color::black()), _textHover(Color::black()), _textPressed(Color::black()) {}

CE_END_NAMESPACE