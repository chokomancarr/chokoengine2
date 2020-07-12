#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

float EUILayout::BeginScroll(const Rect& r, EUILayout::ScrollState& st) {
	UI::PushStencil(r);
	st.rng = r;
	return r.y() + st.off;
}

void EUILayout::EndScroll(EUILayout::ScrollState& st, float o) {
	st.max = o - st.off - st.rng.y();
	o -= st.rng.y();
	if (st.max > st.rng.h()) {
		float dy = (st.rng.Contains(Input::mousePosition())) ?
			10 * Input::mouseScroll().y : 0;
		st.off = Math::Clamp(st.off + dy, st.rng.h() - st.max, 0.f);
	}
	else st.off = 0;
	UI::PopStencil();
}

Vec2 EUILayout::BeginPan(const Rect& r, const Rect& r2, PanState& st) {
	UI::PushStencil(r);
	st.rng = r;
	st.max = Vec2(r2.w(), r2.h());
	return Vec2(r.x(), r.y()) + st.off;
}

void EUILayout::EndPan(EUILayout::PanState& st) {
	
	UI::PopStencil();
}

CE_END_ED_NAMESPACE