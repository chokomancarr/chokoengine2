#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

UIButtonStyle::UIButtonStyle(const Color& n, const Color& h, const Color& p)
        : _normal(n), _hover(h), _pressed(p) {}

CE_END_NAMESPACE