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

void EW_I_Component::Draw(const Component& o, Rect& r) {
    UI::Label(Rect(r.x() + 20, r.y(), r.w() - 22, 16), o->name(), Color(0.8f));
    
    r.y(r.y() + 18);
    _funcs[(int)o->componentType](o, r);
    r.y(r.y() + 2);
}

CE_END_ED_NAMESPACE