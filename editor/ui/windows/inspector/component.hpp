#pragma once
#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"

#define EW_IC_DRAWARGS const Component& _c, UI_Ext::Layout::InfoSt& lt, const pEW_I_Status& _status

CE_BEGIN_ED_NAMESPACE

class EW_I_Component {
    typedef void (*_DrawFn)(EW_IC_DRAWARGS);

    static std::array<_DrawFn, (size_t)ComponentType::_COUNT> _funcs;

#include "ce2/scene/comp/detail/compdefsall.inl"

#define DR(c) static void Draw ## c(EW_IC_DRAWARGS);

    COMP_DEFS_ALL(DR)

#undef DR

#include "ce2/scene/comp/detail/compdefsall_undef.inl"

public:
	static void Draw(EW_IC_DRAWARGS);
};

CE_END_ED_NAMESPACE

#define CE_E_BEGIN_DRAWCOMP(tp) void EW_I_Component::Draw ## tp(EW_IC_DRAWARGS) {\
    tp c = static_cast<tp>(_c);\
    auto status = std::static_pointer_cast<EW_IS_ ## tp>(_status);

#define CE_E_END_DRAWCOMP }