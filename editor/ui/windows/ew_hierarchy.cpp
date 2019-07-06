#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_Hierarchy::DrawMenuObject(int& off, const std::vector<SceneObject>& oo, int level) {
	for (auto& o : oo) {
		auto style = (o == ESceneInfo::selectedObject) ?
			UIButtonStyle(Color(0.2f), Color(0.25f), Color(0.1f)) :
			UIButtonStyle(Color(0, 0), Color(0.3f, 0.5f), Color(0.1f));
		if (UI::I::Button(Rect(position.x() + 1, position.y() + 20 + 17 * off, position.w() - 2, 16), style)
				== InputMouseStatus::HoverUp) {
			ESceneInfo::selectedObject = o;
		}
		UI::Label(Rect(position.x() + 20 + 17 * level, position.y() + 20 + 17 * off, position.w() - 21 - 17 * level, 16), o->name(), Color::gray(0.8f));
		off++;
		DrawMenuObject(off, o->children(), level + 1);
	}
}

void EW_Hierarchy::DrawMenu() {
	if (UI::I::ButtonTr(Rect(position.x() + 1, position.y() + 20, position.w() - 2, position.h() - 21))
			== InputMouseStatus::HoverUp) {
		ESceneInfo::selectedObject = nullptr;
	}
    int off = 0;
    DrawMenuObject(off, Scene::objects(), 0);
}

bool EW_Hierarchy::Init() {
	return true;
}

CE_END_ED_NAMESPACE