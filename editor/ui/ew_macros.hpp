#pragma once

#define CE_E_LBL_W 60

#define CE_E_INC_Y() r.y(r.y() + 18)

#define CE_E_LBL(lbl) UI::Label(Rect(r.x() + 3, r.y(), CE_E_LBL_W - 3, 16), lbl, Color(0.8f))

#define CE_E_F(pr, lbl, nm)\
    UI::Label(Rect(r.x() + 10, r.y(), r.w() * 0.33f, 16), "field of view", Color::white());\
    c->fov(UI_Ext::TextFieldF(Rect(r.x() + r.w() * 0.33f, r.y(), r.w() * 0.67f - 6, 16), c->fov()));\
    r.y(r.y() + 17);


#define CE_E_EDIT_V3(pr, lbl, nm)\
	CE_E_LBL(lbl);\
	const auto& ori_ ## nm = pr nm();\
	auto res_ ## nm = UI_Ext::TextFieldV3(Rect(r.x() + CE_E_LBL_W, r.y(), r.w() - CE_E_LBL_W - 2, 16), ori_ ## nm);\
	if (res_ ## nm != ori_ ## nm) {\
		pr nm(res_ ## nm);\
	}\
	CE_E_INC_Y()