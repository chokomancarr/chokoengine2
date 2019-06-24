#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EWindow::Draw() {
	UI::Rect(Rect(position.x(), position.y(), position.w(), 20), Color(0, 0.7f));
	UI::Label(Rect(position.x() + 2, position.y() + 2, 200, 16), "Window Title", Color::gray(0.7f));

	UI::Rect(Rect(position.x(), position.y() + 20, position.w(), position.h() - 20), Color(0.1f, 0.5f));

	DrawMenu();
}

CE_END_ED_NAMESPACE