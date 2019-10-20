#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/camera.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Camera)
    CE_E_EDIT_F_RNG_FV(c->, "FoV", fov, 1, 179);
	CE_E_EDIT_F_RNG_FV(c->, "Near Clip", nearClip, 0.00001f, c->farClip());
	CE_E_EDIT_F_RNG_FV(c->, "Far Clip", farClip, c->nearClip(), 1000000);
	static Texture tx = nullptr;
	CE_E_LBL("asdf");
	if (UI::I::Button(CE_E_VL_RECT, UIButtonStyle(Color(0.2f)), !tx ? "[none]" : tx->assetSignature()) == InputMouseStatus::HoverUp) {
		EO_SelectRef::RegAsset(tx);
	}
	CE_E_INC_Y();
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE