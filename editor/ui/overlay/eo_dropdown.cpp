#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EO_Dropdown::Draw() {
    const float w = 100;
    UI::Rect(Rect(pos.x, pos.y, w, 20 + 17 * menu.items.size()), Color(0.1f, 0.7f));
    UI::Label(Rect(pos.x + 2, pos.y, w - 4, 16), menu.title, Color(0.5f));
    UI::Line(pos + Vec2(1, 18), pos + Vec2(w - 2, 18), Color(0.5f));
}

void EO_Dropdown::Reg(const Vec2& pos, const EDropdownMenu& menu) {
    const auto& me = GetInstance<EO_Dropdown>();
    me->pos = pos;
    me->menu = menu;
    me->active = true;
}

CE_END_ED_NAMESPACE