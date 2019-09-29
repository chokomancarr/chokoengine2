#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::array<EW_I_Component::_DrawFn, (size_t)ComponentType::_COUNT> EW_I_Component::_funcs = {
    DrawAnimator,
    DrawCamera,
    DrawLight,
	nullptr,
    DrawMeshRenderer,
    DrawRig,
    DrawScript
};

void EW_I_Component::Draw(EW_IC_DRAWARGS) {
    UI::Label(Rect(r.x() + 20, r.y(), r.w() - 22, 16), _c->name(), Color(0.8f));
    
    r.y(r.y() + 18);
	const auto& f = _funcs[(int)_c->componentType];
	if (f) {
		f(_c, r, _status);
	}
	else {
		UI::Label(Rect(r.x() + 10, r.y(), r.w() - 15, 16), "Not implemented!", Color::red());
		r.y(r.y() + 17);
	}
	r.y(r.y() + 2);
}

CE_END_ED_NAMESPACE