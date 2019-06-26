#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_I_SceneObject::Draw(const SceneObject& o, Rect r) {
	const auto nm = o->name();
	UI::Label(Rect(r.x() + 20, r.y(), r.w() - 22, 16), "Object: " + nm, Color(0.8f));

	UI::Label(Rect(r.x() + 20, r.y() + 17, r.w() - 22, 16), "Position: " + o->transform()->localPosition().str(), Color(0.8f));
}

CE_END_ED_NAMESPACE