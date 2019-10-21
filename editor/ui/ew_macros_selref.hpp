#pragma once
#include "ext/ui_ext.hpp"
#include "ew_macros.hpp"

#define CE_E_ASSET_SIG(nm) !nm ? "[none]" : nm->assetSignature()

#define CE_E_ASSET_SELECT(nm) {\
	const auto& _vl = nm();\
	if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)), CE_E_ASSET_SIG(_vl)) == InputMouseStatus::HoverUp) {\
			EO_SelectRef::RegAsset(_vl, std::function<void(const decltype(_vl)&)>([&](const decltype(_vl)& vl) {\
				nm(vl);\
		}));\
	}\
}

#define CE_E_ASSET_SELECT_FV(nm) {\
	if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)), CE_E_ASSET_SIG(nm)) == InputMouseStatus::HoverUp) {\
			EO_SelectRef::RegAsset(nm, std::function<void(const decltype(nm)&)>([&](const decltype(nm)& vl) {\
				nm = vl;\
		}));\
	}\
}

#define CE_E_ASSET_SEEK_BTN(nm) \
if (UI::I::Button(Rect(lt.x + lt.w - 18, lt.y, 16, 16), UIButtonStyle(Color(0.2f))) == InputMouseStatus::HoverUp) {\
	\
}

#define CE_E_ASSET_REF(nm, vl)\
	CE_E_LBL(nm);\
	CE_E_ASSET_SELECT(vl);\
	CE_E_ASSET_SEEK_BTN();\
	CE_E_INC_Y()

#define CE_E_ASSET_REF_FV(nm, vl)\
	CE_E_LBL(nm);\
	CE_E_ASSET_SELECT_FV(vl);\
	CE_E_ASSET_SEEK_BTN();\
	CE_E_INC_Y()