#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Camera)
    UI::Label(Rect(r.x() + 10, r.y(), r.w() - 16, 16), "adsfasdf", Color::white());
    r.y(r.y() + 17);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE