#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EW_I_Status::UMap EW_Inspector::statuses = EW_I_Status::UMap();

void EW_Inspector::DrawMenu() {
	if (!ESceneInfo::selectedObject) {
		UI::Label(Rect(position.x() + 2, position.y() + 22, position.w() - 4, 16), "No object selected", Color(0.8f, 0.5f));
		return;
	}
	EW_I_SceneObject::Draw(ESceneInfo::selectedObject, position + Rect(0, 20, 0, -20));
}

bool EW_Inspector::Init() {
	return true;
}

CE_END_ED_NAMESPACE