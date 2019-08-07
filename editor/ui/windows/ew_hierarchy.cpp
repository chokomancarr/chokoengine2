#include "chokoeditor.hpp"
#include "inspector/status/status_sceneobject.hpp"

CE_BEGIN_ED_NAMESPACE

bool EW_Hierarchy::DrawMenuObject(int& off, const std::vector<SceneObject>& oo, int level) {
	const auto dx = 5 * level + 1;
	bool active = false;
	for (auto& o : oo) {
		auto st = EW_Inspector::statuses.Get<EW_IS_SceneObject>(o->id());
		auto isa = (o == ESceneInfo::selectedObject);
		auto style = isa ?
			UIButtonStyle(Color(0.2f), Color(0.25f), Color(0.1f)) :
			UIButtonStyle(Color(0, 0), Color(0.3f, 0.5f), Color(0.1f));
		const bool hc = !!o->children().size();
		bool nhe = true;
		if (hc) {
			auto hb = UI::I::Button(Rect(position.x() + dx, position.y() + 20 + 17 * off, 16, 16), style);
			nhe = (hb == InputMouseStatus::None);
			if (hb == InputMouseStatus::HoverUp) {
				st->expanded = !st->expanded;
			}
		}
		if (nhe && UI::I::Button(Rect(position.x() + 1, position.y() + 20 + 17 * off, position.w() - 2, 16), style)
				== InputMouseStatus::HoverUp) {
			ESceneInfo::selectedObject = o;
			isa = true;
		}
		UI::Label(Rect(position.x() + dx + 17, position.y() + 20 + 17 * off, position.w() - dx - 18, 16), o->name(), Color::gray(0.8f));
		
		const auto off0 = ++off;
		if (hc && st->expanded) {
			bool ha = DrawMenuObject(off, o->children(), level + 1);
			UI::Rect(Rect(position.x() + dx + 2, position.y() + 20 + 17 * off0, 1, 17 * (off - off0)), Color(0.7f, ha? 0.6f : 0.3f));
		}
		active |= isa;
	}
	return active;
}

void EW_Hierarchy::DoExpandCollapse(bool e, const std::vector<SceneObject>& oo) {
	for (auto& o : oo) {
		auto st = EW_Inspector::statuses.Get<EW_IS_SceneObject>(o->id());
		st->expanded = e;
		DoExpandCollapse(e, o->children());
	}
}

void EW_Hierarchy::DrawMenu() {
	//if (UI::I::ButtonTr(Rect(position.x() + 1, position.y() + 20, position.w() - 2, position.h() - 21))
	//		== InputMouseStatus::HoverUp) {
	//	ESceneInfo::selectedObject = nullptr;
	//}
    int off = 0;
    DrawMenuObject(off, Scene::objects(), 0);
}

bool EW_Hierarchy::Init() {
	return true;
}

void EW_Hierarchy::ExpandAll() {
	DoExpandCollapse(true, Scene::objects());
}

void EW_Hierarchy::CollapseAll() {
	DoExpandCollapse(false, Scene::objects());
}

CE_END_ED_NAMESPACE