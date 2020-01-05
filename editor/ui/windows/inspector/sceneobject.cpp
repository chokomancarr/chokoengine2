#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "status/status_sceneobject.hpp"
#include "status/status_component_base.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_I_SceneObject::Draw(const SceneObject& o, Rect r) {
	auto status = EW_Inspector::statuses.Get<EW_IS_SceneObject>(o->id());

	const auto nm = o->name();
	auto tr = o->transform();

	o->name(UI::I::TextField(Rect(r.x() + 20, r.y(), r.w() - 22, 16), nm, Color()));

	r.y(r.y() + 20);

	static UI_Ext::Layout::InfoSt lt = {};
	UI_Ext::Layout::BeginLayout(r.sub(2, 0, 2, 0), lt);

	UI_Ext::Layout::Block("Transform", lt, [&]() {
		CE_E_EDIT_V3_FV(tr->, "Position", localPosition);
		CE_E_EDIT_V3_FV(tr->, "Rotation", localRotationEuler);
		CE_E_EDIT_V3_FV(tr->, "Scale", localScale);
	});

	for (auto& c : o->components()) {
		auto cstatus = status->components.Get<EW_IS_Component>(c->id());
		EW_I_Component::Draw(c, lt, cstatus);
	}

	const auto ra = Rect(lt.x + 4, lt.y, lt.w - 8, 16);
	if (UI::I::Button(ra, Color(0.3f), "Add Component") == InputMouseStatus::HoverUp) {
		EO_AddComp::Reg(ra, o);
	}
}

CE_END_ED_NAMESPACE