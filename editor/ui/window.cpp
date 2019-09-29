#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EWindow::Draw() {
	UI::Rect(Rect(position.x(), position.y(), position.w(), 20), Color(0, 0.7f));
	UI::Label(Rect(position.x() + 2, position.y() + 2, 200, 16), "Window Title", Color(0.7f));

	if (position.Contains(Input::mousePosition())) {
		UI::defaultFont()->alignment(FontAlign::MiddleCenter);
		float xoff = position.x2() - 50;
		for (auto m : menus) {
			UI::Label(Rect(xoff, position.y() + 2, 50, 16), m.title, Color(0.5f));
			xoff -= 50;
		}
		UI::defaultFont()->alignment(FontAlign::TopLeft);
	}

	UI::Rect(Rect(position.x(), position.y() + 20, position.w(), position.h() - 20), Color(0.1f, 0.5f));

	DrawMenu();
}

CE_END_ED_NAMESPACE