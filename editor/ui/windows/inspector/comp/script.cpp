#include "chokoeditor.hpp"
#include "ui/windows/inspector/status/comp/script.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Script)

DummyScript scr = (DummyScript)c;
if (!scr) return;
for (size_t a = 0, n = scr->vals.size(); a < n; a++) {
	auto& vr = scr->info()->vars[a];
	auto& vl = scr->vals[a];
	switch (vr.type) {
	case ScriptVar::Type::Bool: {
		CE_E_EDIT_TG(vl., vr.name, val_b);
		break;
	}
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
	default:
		CE_E_LBL(vr.name);
		UI::Label(CE_E_VL_RECT, vr.typeName, Color(0.7f));
		CE_E_INC_Y();
		break;
	}
}

CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE