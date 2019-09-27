#pragma once
#include "ext/ui_ext.hpp"

#define CE_E_LBL_W 60

#define CE_E_INC_Y() r.y(r.y() + 18)

#define CE_E_GETVAL_FV(vr) vr()
#define CE_E_SETVAL_FV(vr, vl) vr(vl)

#define CE_E_GETVAL(vr) vr
#define CE_E_SETVAL(vr, vl) vr = vl

#define CE_E_LBL(lbl) UI::Label(Rect(r.x() + 3, r.y(), CE_E_LBL_W - 3, 16), lbl, Color(0.8f))

#define CE_E_VL_RECT Rect(r.x() + CE_E_LBL_W, r.y(), r.w() - CE_E_LBL_W - 2, 16)

#define CE_E_EDIT_ST(tp, pr, lbl, nm, gt, st)\
	CE_E_LBL(lbl);\
	const auto ori_ ## nm = gt(pr nm);\
	const auto res_ ## nm = UI_Ext::TextField ## tp(CE_E_VL_RECT, ori_ ## nm);\
	if (res_ ## nm != ori_ ## nm) {\
		st;\
	}\
	CE_E_INC_Y()

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

#define CE_E_EDIT_TG_FV(pr, lbl, nm)\
	CE_E_LBL(lbl);\
	const auto ori_ ## nm = CE_E_GETVAL_FV(pr nm);\
	const auto res_ ## nm = UI::I::Toggle(Rect(r.x() + r.w() - 18, r.y(), 16, 16), ori_ ## nm, Color(0.2f));\
	if (res_ ## nm != ori_ ## nm) {\
		CE_E_SETVAL_FV(pr nm, res_ ## nm);\
	}\
	CE_E_INC_Y()