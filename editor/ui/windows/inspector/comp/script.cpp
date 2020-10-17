#include "chokoeditor.hpp"
#include "ui/windows/inspector/status/comp/script.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	void DrawVar(const ScriptInfo& info, ScriptVar& vr, ScriptVal& vl, UI_Ext::Layout::InfoSt& lt) {
		if (vr.is_vector) {
			int len = (int)vl.val_vec.size();
			UI_Ext::Layout::Block(vr.name + " (size=" + std::to_string(len) + ")", lt, [&]() {
				auto vr2 = vr;
				vr2.is_vector = false;
				CE_E_EDIT_I(, "size", len);
				vl.val_vec.resize(len, ScriptVal(vr2, info));
				int a = 0;
				for (auto& vl2 : vl.val_vec) {
					vr2.name = std::to_string(a++);
					DrawVar(info, vr2, vl2, lt);
				}
			});
			return;
		}
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
			auto pvl = &vl;
			CE_E_LBL(vr.name);
			if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)), CE_E_ASSET_SIG(vl.val_asset)) == InputMouseStatus::HoverUp) {
				auto pvl = &vl;
				EO_SelectRef::RegAsset(vl.val_asset, std::function<void(const Asset&)>([pvl](const Asset& vv) {
					pvl->val_asset = vv;
				}), vl.var.type_asset);
			}
			else
				CE_E_ASSET_DROP(CE_E_VL_RECT.sub(0, 0, 34, 0), vl.var.type_asset, vl.val_asset = res)
			CE_E_ASSET_SEEK_BTN(vl.val_asset);
			CE_E_INC_Y();
			//*/
			break;
		}
		case ScriptVar::Type::Component: {
			auto pvl = &vl;
			CE_E_LBL(vr.name);
			if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)),
					!vl.val_comp ? "[none]" : vl.val_comp->object()->name() + " (" + ComponentTypeStr.at(vr.type_comp) + ")"
					) == InputMouseStatus::HoverUp) {
				auto pvl = &vl;
				EO_SelectRef::RegComp(vl.val_comp, std::function<void(const Component&)>([pvl](const Component& vv) {
					pvl->val_comp = vv;
				}));
			}
			else
				CE_E_COMP_DROP(CE_E_VL_RECT.sub(0, 0, 34, 0), vr.type_comp, vl.val_comp = res)
			CE_E_ASSET_SEEK_BTN(vl.val_asset);
			CE_E_INC_Y();
			//*/
			break;
		}
		case ScriptVar::Type::SceneObject: {
			CE_E_LBL(vr.name);
			InputMouseStatus mst;
			if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)),
				!vl.val_obj ? "[none]" : (vl.val_obj->name() + " (SceneObject)")) == InputMouseStatus::HoverUp) {
			}
			if (EDragDrop::IsSingle() && EDragDrop::type == EDragDrop::Type::SceneObject
				&& (mst = UI::I::ButtonTr(CE_E_VL_RECT.sub(0, 0, 17, 0))) != InputMouseStatus::None) {
				UI::Rect(CE_E_VL_RECT.sub(0, 0, 17, 0), Color(1, 1, 0, 0.5f));
				if (mst == InputMouseStatus::Up) {
					vl.val_obj = (SceneObject)EDragDrop::targetObj[0];
				}
			}
			CE_E_INC_Y();
			break;
		}
		case ScriptVar::Type::Class: {
			UI_Ext::Layout::Block(vr.name, lt, [&]() {
				for (auto& sc : info->classes) {
					if (sc.name == vr.sub_class) {
						for (size_t a = 0; a < sc.vars.size(); a++) {
							DrawVar(info, sc.vars[a], vl.val_class.val[a], lt);
						}
						break;
					}
				}
			});
			break;
		}
		default:
			CE_E_LBL(vr.name);
			UI::Label(CE_E_VL_RECT, vr.name, Color(0.7f));
			CE_E_INC_Y();
			break;
		}
	}
}

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
//CE_E_ASSET_SEEK_BTN();
CE_E_INC_Y();

for (size_t a = 0, n = scr->vals.size(); a < n; a++) {
	//auto& vr = scr->info()->vars[a];
	auto& vl = scr->vals[a];
	DrawVar(_vl, vl.var, vl, lt);
}

CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE