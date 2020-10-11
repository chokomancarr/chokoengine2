#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EW_I_Status::UMap EW_Inspector::statuses = EW_I_Status::UMap();

std::function<void(UI_Ext::Layout::InfoSt&)> EW_Inspector::customDrawer = nullptr;

void EW_Inspector::DrawMenu() {
	static EUILayout::ScrollState st = {};
	const auto rect = Rect(position.x() + 1, position.y() + 20, position.w() - 2, position.h() - 21);
	static UI_Ext::Layout::InfoSt lt = {};
	UI_Ext::Layout::BeginLayout(rect, lt);
	EUILayout::BeginScroll(lt, st, rect.h());

	if (customDrawer) {
		customDrawer(lt);
	}
	else {
		using ST = ESceneInfo::SelType;
		switch (ESceneInfo::selectedType) {
		case ST::None:
			UI::Label(Rect(lt.x + 2, lt.y, lt.w - 2, 16), "No object selected", Color(0.8f, 0.5f));
			break;
		case ST::SceneObject:
			EW_I_SceneObject::Draw(ESceneInfo::selectedObject, lt);
			break;
		case ST::Asset:
			EW_I_Asset::Draw(ESceneInfo::selectedAsset, lt);
			break;
		case ST::String:
			UI::Label(Rect(lt.x + 2, lt.y, lt.w - 2, 16), ESceneInfo::selectedString, Color(0.8f, 0.5f));
			break;
		}
	}
	EUILayout::EndScroll(lt, st);
}

EW_Inspector::EW_Inspector() : EWindow("Inspector") {}

bool EW_Inspector::Init() {
	return true;
}

CE_END_ED_NAMESPACE