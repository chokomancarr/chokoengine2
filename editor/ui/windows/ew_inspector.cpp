#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EW_I_Status::UMap EW_Inspector::statuses = EW_I_Status::UMap();

void EW_Inspector::DrawMenu() {
	using ST = ESceneInfo::SelType;
	switch (ESceneInfo::selectedType) {
	case ST::None:
		UI::Label(Rect(position.x() + 2, position.y() + 22, position.w() - 4, 16), "No object selected", Color(0.8f, 0.5f));
		break;
	case ST::SceneObject:
		EW_I_SceneObject::Draw(ESceneInfo::selectedObject, position + Rect(0, 20, 0, -20));
		break;
	case ST::Asset:

		break;
	case ST::String:
		UI::Label(Rect(position.x() + 2, position.y() + 22, position.w() - 4, 16), ESceneInfo::selectedString, Color(0.8f, 0.5f));
		
		break;
	}
	
}

EW_Inspector::EW_Inspector() : EWindow("Inspector") {}

bool EW_Inspector::Init() {
	return true;
}

CE_END_ED_NAMESPACE