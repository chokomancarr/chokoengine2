#include "chokoeditor.hpp"
#include "ui/windows/inspector/status/comp/script.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Script)

DummyScript scr = (DummyScript)c;
if (!scr) return;

CE_E_LBL("Script");
const auto& _pvl = scr->info();
const auto& _vl = !_pvl ? nullptr : _pvl.lock();
if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)), !!_vl ? _vl->sig : "None")
		== InputMouseStatus::HoverUp) {
	EO_SelectRef::RegScr(_vl, [scr](ScriptInfo vl) {
		scr->info(vl);
	});
}
//else CE_E_ASSET_DROP(CE_E_VL_RECT.sub(0, 0, 17, 0), decltype(_vl), nm(res))
CE_E_ASSET_SEEK_BTN();
CE_E_INC_Y();

for (size_t a = 0, n = scr->vals.size(); a < n; a++) {
	auto& vr = scr->info()->vars[a];
	auto& vl = scr->vals[a];
	switch (vr.type) {
	case ScriptVar::Type::Int: {
		CE_E_EDIT_I(vl., vr.name, val_i);
		break;
	}
	case ScriptVar::Type::Float: {
		CE_E_EDIT_F(vl., vr.name, val_f);
		break;
	}
	case ScriptVar::Type::Vec3: {
		CE_E_EDIT_V3(vl., vr.name, val_v3);
		break;
	}
	case ScriptVar::Type::Vec4: {
		CE_E_EDIT_V4(vl., vr.name, val_v4);
		break;
	}
	case ScriptVar::Type::Asset: {
		CE_E_LBL(vr.name);
		if (UI::I::Button(CE_E_VL_RECT, UIButtonStyle(Color(0.2f)), CE_E_ASSET_SIG(vl.val_asset)) == InputMouseStatus::HoverUp) {
			EO_SelectRef::RegAsset(vl.val_asset, std::function<void(const Asset&)>([scr, a](const Asset& vl) {
				scr->vals[a].val_asset = vl;
			}), vl.var.type_asset);
		}
		else CE_E_ASSET_DROP(CE_E_VL_RECT.sub(0, 0, 34, 0), vl.var.type_asset, vl.val_asset = res)
		CE_E_ASSET_SEEK_BTN();
		CE_E_INC_Y();
		break;
	}
	default:
		CE_E_LBL(vr.name);
		UI::Label(CE_E_VL_RECT, vr.name, Color(0.7f));
		CE_E_INC_Y();
		break;
	}
}

CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE