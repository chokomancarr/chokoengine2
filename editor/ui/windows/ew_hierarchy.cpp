#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_Hierarchy::DrawMenuObject(int& off, const std::vector<SceneObject>& oo, int level) {
	for (auto& o : oo) {
		UI::Label(Rect(20 + 17 * level, 50 + 17 * off, 100, 16), o->name(), Color::white());
		off++;
		DrawMenuObject(off, o->children(), level + 1);
	}
}

void EW_Hierarchy::DrawMenu() {
    int off = 0;
    DrawMenuObject(off, ChokoLait::scene()->objects(), 0);
}

CE_END_ED_NAMESPACE