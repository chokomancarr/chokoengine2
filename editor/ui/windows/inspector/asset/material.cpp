#include "chokoeditor.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWASSET(Material)

	DrawMaterial2(obj, lt);

CE_E_END_DRAWASSET

CE_E_BEGIN_DRAWASSET2(Material)
	CE_E_ASSET_REF("shader", obj->shader, obj);
	for (auto& v : obj->variables()) {
		auto pv = &v;
		switch (v.type()) {
		case ShaderVariableType::Float: {
			CE_E_EDIT_F_FV(pv->, v.name(), val_f);
			break;
		}
		case ShaderVariableType::Color: {
			CE_E_EDIT_CL_FV(pv->, v.name(), val_v4);
			break;
		}
		case ShaderVariableType::Texture: {
			CE_E_ASSET_REF(v.name(), pv->val_t, pv);
			break;
		}
		default:
			break;
		}
	}
CE_E_END_DRAWASSET2

CE_END_ED_NAMESPACE
