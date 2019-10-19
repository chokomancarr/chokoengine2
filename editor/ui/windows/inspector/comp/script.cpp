#include "chokoeditor.hpp"
#include "ui/windows/inspector/status/comp/script.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Script)

DummyScript scr = (DummyScript)c;
for (auto& v : scr->info->vars) {
	switch (v.type) {
	default:
		CE_E_LBL(v.name);
		CE_E_INC_Y();
		break;
	}
}

CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE