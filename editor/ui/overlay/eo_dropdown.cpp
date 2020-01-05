#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::shared_ptr<EO_Dropdown> EO_Dropdown::instance = nullptr;

void EO_Dropdown::DoDraw(bool title) {
	static UIButtonStyle style(
		Color(0, 0), Color(0.3f, 0.5f), Color(0, 0.5f),
		Color(1), Color(1), Color(1)
	);

	const float w = 150;
	const auto isz = menu.items.size();
	const auto r = Rect(pos.x, pos.y, w, (title ? 20 : 2) + 17 * isz);
	UI::Rect(r, Color(0.1f, 0.7f));
	float y = pos.y + 2;
	if (title) {
		UI::Label(Rect(pos.x + 2, pos.y, w - 4, 16), menu.title, Color(0.5f));
		UI::Rect(Rect(pos.x + 1, pos.y + 18, w - 2, 1), Color(0.5f));
		y += 18;
	}
	for (int a = 0; a < isz; a++) {
		if (UI::I::Button(Rect(pos.x + 2, y + a * 17, w - 4, 17),
				style,
				menu.items[a].title) == InputMouseStatus::HoverUp) {
			menu.items[a].callback();
			active = false;
		}
	}
	if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up && !r.Contains(Input::mouseDownPosition())) {
		active = false;
	}
}

void EO_Dropdown::Draw() {
	DoDraw(title);
}

void EO_Dropdown::Reg(const Vec2& pos, const EDropdownMenu& menu, bool title) {
    const auto& me = GetInstance<EO_Dropdown>();
    me->pos = pos;
    me->menu = menu;
	me->title = title;
    me->active = true;
}

void EO_Dropdown::DrawMenus(const std::vector<EDropdownMenu>& menus, Vec2 pos, bool right) {
	if (right) pos.x -= menus.size() * 50;

	UI::defaultFont()->alignment(FontAlign::TopCenter);
	for (auto m : menus) {
		UI::Label(Rect(pos.x, pos.y, 50, 16), m.title, Color(0.5f, m.enabled ? 1 : 0.5f));
		if (m.enabled && UI::I::ButtonTr(Rect(pos.x, pos.y, 50, 16)) == InputMouseStatus::HoverUp) {
			EO_Dropdown::Reg(Vec2(pos.x, pos.y + 18), m, false);
		}
		pos.x += 50;
	}
	UI::defaultFont()->alignment(FontAlign::TopLeft);
}

CE_END_ED_NAMESPACE