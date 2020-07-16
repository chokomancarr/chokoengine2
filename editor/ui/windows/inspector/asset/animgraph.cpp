#include "chokoeditor.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWASSET(AnimGraph)

UI::Label(Rect(lt.x + 2, lt.y, lt.w, 16), "num states: " + std::to_string(obj->nodes().size()), Color(0.5f));
CE_E_INC_Y();

CE_E_LBL("variables");
CE_E_INC_Y();
UI_Ext::Layout::Block("", lt, [&]() {
	for (auto& vr : obj->vars()) {
		UI::Texture(Rect(lt.x + 3, lt.y, 16, 16), EIcons::icons["close"], Color::red());
		vr.name = UI::I::TextField(Rect(lt.x + 20, lt.y, lt.w / 2 + 10, 16), vr.name, Color(0.2f));
		const std::string tpstrs[] = {
			"bool", "int", "float"
		};
		if (UI::I::Button(Rect(lt.x + lt.w / 2 + 32, lt.y, lt.w / 2 - 34, 16), Color(0.2f), tpstrs[(int)vr.type]) == InputMouseStatus::HoverUp) {
			EO_SelectEnum::RegEnumStr(Rect(lt.x + lt.w / 2 + 1, lt.y, lt.w / 2 - 2, 16), vr.type, tpstrs);
		}
		CE_E_INC_Y();
		CE_E_EDIT_TG(vr.defaultValue., "value", b);
	}
	CE_E_LIST_ADD(obj->vars(), _AnimGraph::Var());
});


CE_E_END_DRAWASSET

CE_END_ED_NAMESPACE
