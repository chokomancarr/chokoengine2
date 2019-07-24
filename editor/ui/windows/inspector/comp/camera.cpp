#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/camera.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Camera)
    UI::Label(Rect(r.x() + 10, r.y(), r.w() * 0.33f, 16), "field of view", Color::white());
    c->fov(UI_Ext::TextFieldF(Rect(r.x() + r.w() * 0.33f, r.y(), r.w() * 0.67f - 6, 16), c->fov()));
    r.y(r.y() + 17);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE