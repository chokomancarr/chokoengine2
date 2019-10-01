#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::array<EW_I_Component::_DrawFn, (size_t)ComponentType::_COUNT> EW_I_Component::_funcs = {
    DrawAnimator,
    DrawCamera,
    DrawLight,
	DrawLightProbe,
    DrawMeshRenderer,
    DrawRig,
    DrawScript
};

void EW_I_Component::Draw(EW_IC_DRAWARGS) {
	UI_Ext::Layout::Push(_c->name(), lt);
    
	const auto& f = _funcs[(int)_c->componentType];
	if (f) {
		f(_c, lt, _status);
	}
	else {
		UI::Label(Rect(lt.x + 10, lt.y, lt.w - 15, 16), "Not implemented!", Color::red());
		lt.y += 17;
	}

	UI_Ext::Layout::Pop(lt);
}

CE_END_ED_NAMESPACE