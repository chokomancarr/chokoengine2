#include "chokoeditor.hpp"
#include "inspector/status/status_sceneobject.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	EDropdownMenu menu_obj;
	EDropdownMenu menu_empty;
}

float EW_Hierarchy::DrawMenuObject(float& off, const std::vector<SceneObject>& oo, int level) {
	const auto dx = 5 * level + 2;
	float ol = 0;
	for (auto& o : oo) {
		auto st = EW_Inspector::statuses.Get<EW_IS_SceneObject>(o->id());
		auto isa = (o == ESceneInfo::selectedObject);
		auto style = isa ?
			UIButtonStyle(Color(0.2f), Color(0.25f), Color(0.1f)) :
			UIButtonStyle(Color(0, 0), Color(0.3f, 0.5f), Color(0.1f));
		const bool hc = !!o->children().size();
		bool prs = false;
		if (hc) {
			auto hb = UI::I::ButtonTr(Rect(position.x() + dx, off, 16, 16));
			prs = !!((int)(hb) & 0x0f);
			if (hb == InputMouseStatus::HoverUp) {
				st->expanded = !st->expanded;
			}
		}
		const Rect orect(position.x() + 1, off, position.w() - 2, 16);
		if (!prs && UI::I::Button(orect, style) == InputMouseStatus::HoverUp) {
			ESceneInfo::selectedObject = o;
			isa = true;
		}
		else if (Input::mouseStatus(InputMouseButton::Right) == InputMouseStatus::Up
				&& orect.Contains(Input::mousePosition())) {
			ESceneInfo::selectedObject = o;
			EO_Dropdown::Reg(Input::mousePosition() + Vec2(1, 1), menu_obj, false);
		}
		if (hc) {
			UI::Texture(Rect(position.x() + dx, off, 16, 16), EIcons::icons[st->expanded ? "minus" : "plus"], Color(0.8f));
		}
		UI::Label(Rect(position.x() + dx + 17, off, position.w() - dx - 18, 16), o->name(), Color(0.8f));
		
		if (level > 0) {
			ol = off + 8;
			UI::Rect(Rect(position.x() + dx - 4, ol, hc ? 5 : 15, 1), Color(0.7f, 0.3f));
		}

		const auto off0 = off += 17;
		if (hc && st->expanded) {
			auto ol = DrawMenuObject(off, o->children(), level + 1);
			UI::Rect(Rect(position.x() + dx, off0, 1, ol - off0), Color(0.7f, 0.3f));
		}
	}
	return ol;
}

void EW_Hierarchy::DoExpandCollapse(bool e, const std::vector<SceneObject>& oo) {
	for (auto& o : oo) {
		auto st = EW_Inspector::statuses.Get<EW_IS_SceneObject>(o->id());
		st->expanded = e;
		DoExpandCollapse(e, o->children());
	}
}

void EW_Hierarchy::DrawMenu() {
	static EUILayout::ScrollState st = {};
	const auto rect = Rect(position.x() + 1, position.y() + 20, position.w() - 2, position.h() - 21);
	float off = EUILayout::BeginScroll(rect, st);
	DrawMenuObject(off, ChokoEditor::scene->objects()[1]->children(), 0);

	if (off < rect.y2()) {
		const auto rect2 = rect.sub(0, off - rect.y(), 0, 0);
		if (rect2.Contains(Input::mousePosition())) {
			if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up) {
				ESceneInfo::selectedObject = nullptr;
			}
			else if (Input::mouseStatus(InputMouseButton::Right) == InputMouseStatus::Up) {
				ESceneInfo::selectedObject = nullptr;
				EO_Dropdown::Reg(Input::mousePosition() + Vec2(1, 1), menu_empty, false);
			}
		}
	}

	off += 17;
	
	EUILayout::EndScroll(st, off);
}

EW_Hierarchy::EW_Hierarchy() : EWindow("Hierarchy") {}

bool EW_Hierarchy::Init() {
#define addi(nm, ...) {auto op = EDropdownMenu(#nm);\
		op.callback = ECallbackCaller(__VA_ARGS__);\
		menu->push_back(op);}

	auto menu = &menu_obj.items;

	addi(New child Object, CallbackSig::OBJECT_NEW_CHILD);
	addi(Delete, CallbackSig::OBJECT_DELETE);

	menu = &menu_empty.items;

	addi(New Object, CallbackSig::OBJECT_NEW);
	addi(Collapse);

	return true;
}

void EW_Hierarchy::ExpandAll() {
	DoExpandCollapse(true, ChokoEditor::scene->objects());
}

void EW_Hierarchy::CollapseAll() {
	DoExpandCollapse(false, ChokoEditor::scene->objects());
}

CE_END_ED_NAMESPACE