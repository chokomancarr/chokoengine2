#include "chokoeditor.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

std::array<EW_I_Asset::_DrawFn, (size_t)AssetType::_COUNT> EW_I_Asset::_funcs = {
	DrawAnimClip,
	DrawAnimGraph,
	DrawArmature,
	DrawBackground,
	DrawFont,
	DrawMaterial,
	DrawMesh,
	DrawPrefab,
	DrawShader,
	DrawTexture
};

void EW_I_Asset::Draw(const Asset& o, Rect r) {
	static auto lt = UI_Ext::Layout::InfoSt();
	UI_Ext::Layout::BeginLayout(r.sub(2, 0, 2, 0), lt);

	CE_E_LBL(o->name());
	CE_E_INC_Y();

	_funcs[(int)o->assetType](o, lt);

	UI_Ext::Layout::EndLayout(lt);
}

CE_END_ED_NAMESPACE