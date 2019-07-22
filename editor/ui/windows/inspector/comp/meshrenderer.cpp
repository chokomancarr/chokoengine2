#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(MeshRenderer)
    static float dy = 0;
    const float y0 = r.y() + 17;
    UI::Rect(Rect(r.x() + 4, r.y(), r.w() - 8, 17), Color(0.1f, 0.7f));
	UI::Rect(Rect(r.x() + 4, y0, r.w() - 8, dy), Color(0.1f, 0.3f));
    UI::Label(Rect(r.x() + 10, r.y(), r.w() * 0.33f, 16), "Modifiers", Color::white());
    r.y(r.y() + 17);
    for (auto& m : c->modifiers()) {
        UI::Label(Rect(r.x() + 14, r.y(), r.w() * 0.33f, 16), m->name(), Color::white());
        r.y(r.y() + 17);
    }
    dy = r.y() - y0;
    r.y(r.y() + 1);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE