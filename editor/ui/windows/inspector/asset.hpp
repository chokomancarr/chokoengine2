#pragma once
#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"

#define EW_IA_DRAWARGS const Asset& _obj, UI_Ext::Layout::InfoSt& lt

#define CE_EI_ASSET_DEF(tp) static void Draw ## tp(EW_IA_DRAWARGS);

#define CE_E_BEGIN_DRAWASSET(tp) void EW_I_Asset::Draw ## tp(EW_IA_DRAWARGS) {\
    tp obj = static_cast<tp>(_obj);

#define CE_E_END_DRAWASSET }

CE_BEGIN_ED_NAMESPACE

class EW_I_Asset {
public:
	static void Draw(const Asset& o, Rect r);

private:
	typedef void(*_DrawFn)(EW_IA_DRAWARGS);

	static std::array<_DrawFn, (size_t)AssetType::_COUNT> _funcs;

	CE_EI_ASSET_DEF(AnimClip)
	CE_EI_ASSET_DEF(AnimGraph)
	CE_EI_ASSET_DEF(Armature)
	CE_EI_ASSET_DEF(Background)
	CE_EI_ASSET_DEF(Font)
	CE_EI_ASSET_DEF(Material)
	CE_EI_ASSET_DEF(Mesh)
	CE_EI_ASSET_DEF(Prefab)
	CE_EI_ASSET_DEF(Shader)
	CE_EI_ASSET_DEF(Texture)
};

#undef CE_EI_ASSET_DEF

CE_END_ED_NAMESPACE