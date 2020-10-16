#include "chokoeditor.hpp"
#include "ce2/scene/comp/detail/compdefsall.inl"

CE_BEGIN_ED_NAMESPACE

#define CM(c) Draw ## c,

std::array<EW_I_Component::_DrawFn, (size_t)ComponentType::_COUNT> EW_I_Component::_funcs = {
    COMP_DEFS_ALL(CM)
};

void EW_I_Component::Draw(EW_IC_DRAWARGS) {
	UI_Ext::Layout::Block(_c->name(), lt, [&]() {
		if ((int)_c->componentType >= (int)ComponentType::_COUNT) return;
		const auto& f = _funcs[(int)_c->componentType];
		if (f) {
			f(_c, lt, _status);
		}
		else {
			UI::Label(Rect(lt.x + 10, lt.y, lt.w - 15, 16), "Not implemented!", Color::red());
			lt.y += 17;
		}
	});
}

CE_END_ED_NAMESPACE