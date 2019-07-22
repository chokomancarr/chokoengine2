#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<float> EW_I_SceneObject::_coffs;

void EW_I_SceneObject::Draw(const SceneObject& o, Rect r) {
	const auto nm = o->name();
	o->name(UI::I::TextField(Rect(r.x() + 20, r.y(), r.w() - 22, 16), nm, Color()));

	r.y(r.y() + 20);

	UI_Ext::TextFieldV3(Rect(r.x() + 26, r.y(), r.w() - 28, 16), o->transform()->localPosition());

	r.y(r.y() + 20);

	_coffs.resize(o->components().size());

	float* _coff = _coffs.data();
	for (auto& c : o->components()) {
		float y0 = r.y() + 17;
	    UI::Rect(Rect(r.x() + 2, r.y(), r.w() - 4, 17), Color(0.1f, 0.7f));
		UI::Rect(Rect(r.x() + 2, y0, r.w() - 4, *_coff), Color(0.1f, 0.3f));
		EW_I_Component::Draw(c, r);
		*_coff = r.y() - y0;
		r.y(r.y() + 2);
		_coff++;
	}
}

CE_END_ED_NAMESPACE