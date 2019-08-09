#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

float EUILayout::BeginScroll(const Rect& r, EUILayout::ScrollState& st) {
	UI::PushStencil(r);
	st.rng = r;
	return r.y() + st.off;
}

void EUILayout::EndScroll(EUILayout::ScrollState& st, float o) {
	st.max = o - st.off;
	o -= st.rng.y();
	if (st.max > st.rng.h()) {
		st.off = Math::Clamp(st.off + 10 * Input::mouseScroll().y, st.rng.h() - st.max, 0.f);
	}
	else st.off = 0;
	UI::PopStencil();
}

CE_END_ED_NAMESPACE