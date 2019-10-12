#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EWindow::EWindow(const std::string& title) : title(title) {}

void EWindow::Draw() {
	UI::Rect(Rect(position.x(), position.y(), position.w(), 20), Color(0, 0.7f));
	UI::Label(Rect(position.x() + 2, position.y() + 2, 200, 16), title, Color(0.7f));

	if (position.Contains(Input::mousePosition())) {
		UI::defaultFont()->alignment(FontAlign::TopCenter);
		float xoff = position.x2() - 50;
		for (auto m : menus) {
			UI::Label(Rect(xoff, position.y() + 2, 50, 16), m.title, Color(0.5f, m.enabled ? 1 : 0.5f));
			if (m.enabled && UI::I::ButtonTr(Rect(xoff, position.y() + 2, 50, 16)) == InputMouseStatus::HoverUp) {
				EO_Dropdown::Reg(Vec2(xoff, position.y() + 18), m, false);
			}
			xoff -= 50;
		}
		UI::defaultFont()->alignment(FontAlign::TopLeft);
	}

	UI::Rect(Rect(position.x(), position.y() + 20, position.w(), position.h() - 20), Color(0.1f, 0.5f));

	DrawMenu();
}

CE_END_ED_NAMESPACE