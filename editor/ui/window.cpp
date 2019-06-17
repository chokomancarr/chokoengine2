#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EWindow::Draw() {
	UI::Rect(Rect(position.x(), position.y(), position.w(), 20), EColors::accent);
	UI::Rect(Rect(position.x(), position.y() + 21, position.w(), position.h() - 21), Color(0.1f, 0.5f));

	DrawMenu();
}

CE_END_ED_NAMESPACE