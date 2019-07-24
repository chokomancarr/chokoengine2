#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "status/status_sceneobject.hpp"
#include "status/status_component_base.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_E_EDIT_V3(pr, nm)\
	const auto& ori_ ## nm = pr nm();\
	auto res_ ## nm = UI_Ext::TextFieldV3(Rect(r.x() + 26, r.y(), r.w() - 28, 16), ori_ ## nm);\
	if (res_ ## nm != ori_ ## nm) {\
		pr nm(res_ ## nm);\
	}\
	r.y(r.y() + 18)

void EW_I_SceneObject::Draw(const SceneObject& o, Rect r) {
	auto status = EW_Inspector::statuses.Get<EW_IS_SceneObject>(o->id());

	const auto nm = o->name();
	auto tr = o->transform();

	o->name(UI::I::TextField(Rect(r.x() + 20, r.y(), r.w() - 22, 16), nm, Color()));

	r.y(r.y() + 20);

	CE_E_EDIT_V3(tr->, localPosition);
	CE_E_EDIT_V3(tr->, localScale);

	for (auto& c : o->components()) {
		auto cstatus = status->components.Get<EW_IS_Component>(c->id());
		float y0 = r.y() + 17;
	    UI::Rect(Rect(r.x() + 2, r.y(), r.w() - 4, 17), Color(0.1f, 0.7f));
		UI::Rect(Rect(r.x() + 2, y0, r.w() - 4, cstatus->height), Color(0.1f, 0.3f));
		EW_I_Component::Draw(c, r, cstatus);
		cstatus->height = r.y() - y0;
		r.y(r.y() + 2);
	}
}

CE_END_ED_NAMESPACE