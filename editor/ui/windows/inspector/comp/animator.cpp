#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/animator.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Animator)
	CE_E_EDIT_TG_FV(c->, "single", single);
	if (c->single()) {
		CE_E_ASSET_REF("clip", c->clip, c);
		CE_E_EDIT_TG_FV(c->, "repeat", repeat);
	}
	else {
		CE_E_ASSET_REF("graph", c->graph, c);
	}
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE