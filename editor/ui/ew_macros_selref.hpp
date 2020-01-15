#pragma once
#include "ext/ui_ext.hpp"
#include "ext/traits_ext.hpp"
#include "ew_macros.hpp"

#define CE_E_ASSET_SIG(nm) !nm ? "[none]" : nm->assetSignature()

#define CE_E_ASSET_DROP(r, tp, fn) {\
	InputMouseStatus mst;\
	if (EDragDrop::IsSingle() && EDragDrop::type == EDragDrop::Type::Asset\
			&& EDragDrop::assetType.subtype == EAssetList::TypeOfSt::Type::Asset\
			&& (mst = UI::I::ButtonTr(r)) != InputMouseStatus::None) {\
		UI::Rect(r, Color(1, 1, 0, 0.5f));\
		if (mst == InputMouseStatus::Up) {\
			auto res = EAssetList::Get(EDragDrop::assetType.assetType, EDragDrop::target[0], true);\
			if (res->assetType == tp) { fn; }\
		}\
	}\
}

#define CE_E_ASSET_SELECT(nm, cp) {\
	const auto& _vl = nm();\
	using _Tp = std::remove_const_ref<decltype(_vl)>::type;\
	if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)), CE_E_ASSET_SIG(_vl)) == InputMouseStatus::HoverUp) {\
			EO_SelectRef::RegAsset(_vl, std::function<void(decltype(_vl))>([cp](decltype(_vl) vl) {\
				nm(vl);\
		}));\
	}\
	else CE_E_ASSET_DROP(CE_E_VL_RECT.sub(0, 0, 17, 0), EAssetTypeOf<_Tp>::value, nm((_Tp)res))\
}

#define CE_E_ASSET_SELECT_FV(nm) {\
	using _Tp = std::remove_reference<decltype(nm)>::type;\
	if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)), CE_E_ASSET_SIG(nm)) == InputMouseStatus::HoverUp) {\
			EO_SelectRef::RegAsset(nm, std::function<void(decltype(nm))>([&](decltype(nm) vl) {\
				nm = vl;\
		}));\
	}\
	else CE_E_ASSET_DROP(CE_E_VL_RECT.sub(0, 0, 17, 0), EAssetTypeOf<_Tp>::value, nm = (_Tp)res)\
}

#define CE_E_ASSET_SEEK_BTN(nm) \
if (UI::I::Button(Rect(lt.x + lt.w - 18, lt.y, 16, 16), UIButtonStyle(Color(0.2f))) == InputMouseStatus::HoverUp) {\
	\
}

#define CE_E_ASSET_REF(nm, vl, cp)\
	CE_E_LBL(nm);\
	CE_E_ASSET_SELECT(vl, cp);\
	CE_E_ASSET_SEEK_BTN();\
	CE_E_INC_Y()

#define CE_E_ASSET_REF_FV(nm, vl)\
	CE_E_LBL(nm);\
	CE_E_ASSET_SELECT_FV(vl);\
	CE_E_ASSET_SEEK_BTN();\
	CE_E_INC_Y()
