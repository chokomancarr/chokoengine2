#pragma once
#include "ext/ui_ext.hpp"

#define CE_E_LBL_W 60

#define CE_E_INC_Y() lt.y += 18

#define CE_E_GETVAL_FV(vr) vr()
#define CE_E_SETVAL_FV(vr, vl) vr(vl)

#define CE_E_GETVAL(vr) vr
#define CE_E_SETVAL(vr, vl) vr = vl

#define CE_E_LBL(lbl) UI::Label(Rect(lt.x + 3, lt.y, CE_E_LBL_W - 3, 16), lbl, Color(0.8f))

#define CE_E_VL_RECT Rect(lt.x + CE_E_LBL_W, lt.y, lt.w - CE_E_LBL_W - 2, 16)
#define CE_E_VL2_RECT Rect(lt.x + lt.w - CE_E_LBL_W - 2, lt.y, CE_E_LBL_W, 16)

#define CE_E_EDIT_ST(tp, pr, lbl, nm, gt, st)\
	CE_E_LBL(lbl);\
	const auto ori_ ## nm = gt(pr nm);\
	const auto res_ ## nm = UI_Ext::TextField ## tp(CE_E_VL_RECT, ori_ ## nm);\
	if (res_ ## nm != ori_ ## nm) {\
		st;\
	}\
	CE_E_INC_Y()

#define CE_E_EDIT_I(pr, lbl, nm)\
	CE_E_EDIT_ST(I, pr, lbl, nm, CE_E_GETVAL, CE_E_SETVAL(pr nm, res_ ## nm))

#define CE_E_EDIT_I_FV(pr, lbl, nm)\
	CE_E_EDIT_ST(I, pr, lbl, nm, CE_E_GETVAL_FV, CE_E_SETVAL_FV(pr nm, res_ ## nm))

#define CE_E_EDIT_F(pr, lbl, nm)\
	CE_E_EDIT_ST(F, pr, lbl, nm, CE_E_GETVAL, CE_E_SETVAL(pr nm, res_ ## nm))

#define CE_E_EDIT_F_FV(pr, lbl, nm)\
	CE_E_EDIT_ST(F, pr, lbl, nm, CE_E_GETVAL_FV, CE_E_SETVAL_FV(pr nm, res_ ## nm))

#define CE_E_EDIT_F_RNG(pr, lbl, nm, mn, mx)\
	CE_E_EDIT_ST(F, pr, lbl, nm, CE_E_GETVAL, CE_E_SETVAL(pr nm, Math::Clamp<float>(res_ ## nm, mn, mx)))

#define CE_E_EDIT_F_RNG_FV(pr, lbl, nm, mn, mx)\
	CE_E_EDIT_ST(F, pr, lbl, nm, CE_E_GETVAL_FV, CE_E_SETVAL_FV(pr nm, Math::Clamp<float>(res_ ## nm, mn, mx)))

#define CE_E_EDIT_V3(pr, lbl, nm)\
	CE_E_EDIT_ST(V3, pr, lbl, nm, CE_E_GETVAL, CE_E_SETVAL(pr nm, res_ ## nm))

#define CE_E_EDIT_V3_FV(pr, lbl, nm)\
	CE_E_EDIT_ST(V3, pr, lbl, nm, CE_E_GETVAL_FV, CE_E_SETVAL_FV(pr nm, res_ ## nm))

#define CE_E_EDIT_V4(pr, lbl, nm)\
	CE_E_EDIT_ST(V4, pr, lbl, nm, CE_E_GETVAL, CE_E_SETVAL(pr nm, res_ ## nm));\
	CE_E_INC_Y()

#define CE_E_EDIT_TG(pr, lbl, nm)\
	CE_E_LBL(lbl);\
	const auto ori_ ## nm = CE_E_GETVAL(pr nm);\
	const auto res_ ## nm = UI::I::Toggle(Rect(lt.x + lt.w - 18, lt.y, 16, 16), ori_ ## nm, Color(0.2f));\
	if (res_ ## nm != ori_ ## nm) {\
		CE_E_SETVAL(pr nm, res_ ## nm);\
	}\
	CE_E_INC_Y()

#define CE_E_EDIT_TG_FV(pr, lbl, nm)\
	CE_E_LBL(lbl);\
	const auto ori_ ## nm = CE_E_GETVAL_FV(pr nm);\
	const auto res_ ## nm = UI::I::Toggle(Rect(lt.x + lt.w - 18, lt.y, 16, 16), ori_ ## nm, Color(0.2f));\
	if (res_ ## nm != ori_ ## nm) {\
		CE_E_SETVAL_FV(pr nm, res_ ## nm);\
	}\
	CE_E_INC_Y()

#define CE_E_EDIT_CL_FV(pr, lbl, nm)\
	CE_E_LBL(lbl);\
	const auto ori_ ## nm = CE_E_GETVAL_FV(pr nm);\
	UI::Rect(CE_E_VL_RECT, ori_ ## nm);\
	if (UI::I::Button(CE_E_VL_RECT, UIButtonStyle(ori_ ## nm)) == InputMouseStatus::HoverUp) {\
		EO_ColorPicker::Reg(CE_E_VL_RECT, ori_ ## nm, std::function<void(const Color&)>([&](const Color& vl) {\
			CE_E_SETVAL_FV(pr nm, vl);\
		}));\
	}\
	CE_E_INC_Y();

#define CE_E_EDIT_CRV_FV(pr, lbl, nm) {\
	CE_E_LBL(lbl);\
	const float _w = (lt.w - CE_E_LBL_W - 2) / 2;\
	const auto& def_vl = pr nm();\
	const auto res_const = UI_Ext::TextFieldF(Rect(lt.x + CE_E_LBL_W, lt.y, _w - 8, 16), def_vl.constant);\
	UI::Label(Rect(lt.x + CE_E_LBL_W + _w - 6, lt.y, _w, 16), "+?", Color(0.8f));\
	const auto res_rand = UI_Ext::TextFieldF(Rect(lt.x + CE_E_LBL_W + _w + 10, lt.y, _w - 10, 16), def_vl.random);\
	if (res_const != def_vl.constant || res_rand != def_vl.random) {\
		pr nm(CRValue(res_const, res_rand));\
	}\
	CE_E_INC_Y();\
}

#define CE_E_EDIT_ENUM(lbl, nm, strs, cp) {\
	CE_E_LBL(lbl);\
	const auto& _vl = nm();\
	using _Tp = std::decay<decltype(_vl)>::type;\
	if (UI::I::Button(CE_E_VL_RECT, UIButtonStyle(Color(0.2f)), strs.at(_vl)) == InputMouseStatus::HoverUp) {\
			EO_SelectEnum::RegEnumStr(CE_E_VL_RECT, _vl, strs, std::function<void(_Tp)>([cp](_Tp vl) {\
				nm(vl);\
		}));\
	}\
}\
CE_E_INC_Y();

#define CE_E_LIST_ADD(vec, con, fn) {\
	if (UI::I::Button(CE_E_VL2_RECT, UIButtonStyle(Color(0.2f)), "Add") == InputMouseStatus::HoverUp) {\
		vec.push_back(con);\
		fn;\
	}\
	CE_E_INC_Y();\
}

#include "ew_macros_selref.hpp"
#include "ew_macros_dropdown.hpp"