#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::array<EW_I_Component::_DrawFn, (size_t)ComponentType::_COUNT> EW_I_Component::_funcs = {
    DrawAnimator,
    DrawCamera,
    nullptr,
    DrawMeshRenderer,
    DrawRig,
    DrawScript
};

void EW_I_Component::Draw(EW_IC_DRAWARGS) {
    UI::Label(Rect(r.x() + 20, r.y(), r.w() - 22, 16), _c->name(), Color(0.8f));
    
    r.y(r.y() + 18);
    _funcs[(int)_c->componentType](_c, r, _status);
    r.y(r.y() + 2);
}

CE_END_ED_NAMESPACE