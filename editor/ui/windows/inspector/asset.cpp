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

void EW_I_Asset::Draw(const Asset& o, UI_Ext::Layout::InfoSt& lt) {
	CE_E_LBL(o->name());
	CE_E_INC_Y();

	if (UI::I::Button(Rect(lt.x, lt.y, 150, 16), Color(0.2f), "Apply")
		== InputMouseStatus::HoverUp) {
		EAssetWriter::Write(o);
	}
	CE_E_INC_Y();
	CE_E_INC_Y();

	_funcs[(int)o->assetType](o, lt);
}

CE_END_ED_NAMESPACE