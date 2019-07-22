#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_I_Component {
    typedef void (*_DrawFn)(const Component& c, Rect& r);

    static std::array<_DrawFn, (size_t)ComponentType::_COUNT> _funcs;

    static void DrawAnimator(const Component& c, Rect& r);
    static void DrawCamera(const Component& c, Rect& r);
    static void DrawMeshRenderer(const Component& c, Rect& r);
    static void DrawRig(const Component& c, Rect& r);
    static void DrawScript(const Component& c, Rect& r);

public:
	static void Draw(const Component& c, Rect& r);
};

CE_END_ED_NAMESPACE

#define CE_E_BEGIN_DRAWCOMP(tp) void EW_I_Component::Draw ## tp(const Component& c, Rect& r) {\

#define CE_E_END_DRAWCOMP }