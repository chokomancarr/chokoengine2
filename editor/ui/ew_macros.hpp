#pragma once

#define CE_E_LBL_W 60

#define CE_E_INC_Y() r.y(r.y() + 18)

#define CE_E_LBL(lbl) UI::Label(Rect(r.x() + 3, r.y(), CE_E_LBL_W - 3, 16), lbl, Color(0.8f))

#define CE_E_SV_VAL(pr, nm) const auto ori_ ## nm = pr nm()

#define CE_E_VL_RECT Rect(r.x() + CE_E_LBL_W, r.y(), r.w() - CE_E_LBL_W - 2, 16)

#define CE_E_EDIT_F(pr, lbl, nm)\
    CE_E_LBL(lbl);\
	CE_E_SV_VAL(pr, nm);\
    const auto res_ ## nm = UI_Ext::TextFieldF(CE_E_VL_RECT, ori_ ## nm);\
    if (res_ ## nm != ori_ ## nm) {\
		pr nm(res_ ## nm);\
	}\
	CE_E_INC_Y()

#define CE_E_EDIT_F_RNG(pr, lbl, nm, mn, mx)\
    CE_E_LBL(lbl);\
	CE_E_SV_VAL(pr, nm);\
    const auto res_ ## nm = UI_Ext::TextFieldF(CE_E_VL_RECT, ori_ ## nm);\
    if (res_ ## nm != ori_ ## nm) {\
		pr nm(Math::Clamp<float>(res_ ## nm, mn, mx));\
	}\
	CE_E_INC_Y()

#define CE_E_EDIT_V3(pr, lbl, nm)\
	CE_E_LBL(lbl);\
	CE_E_SV_VAL(pr, nm);\
	const auto res_ ## nm = UI_Ext::TextFieldV3(CE_E_VL_RECT, ori_ ## nm);\
	if (res_ ## nm != ori_ ## nm) {\
		pr nm(res_ ## nm);\
	}\
	CE_E_INC_Y()