#include "chokoeditor.hpp"
#include "ui/windows/inspector/status/comp/rigidbody.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Rigidbody)
	CE_E_EDIT_TG_FV(c->, "Dynamic", dynamic);
	CE_E_EDIT_F_FV(c->, "Mass", mass);
	//c->mass(std::max(c->mass(), 0.00001f));
	CE_E_EDIT_F_FV(c->, "Moment", moment);
	//c->moment(std::max(c->moment(), 0.00001f));
	CE_E_EDIT_F_FV(c->, "Bounce", bounce);

	UI_Ext::Layout::Block("Dynamics", lt, [&]() {
		CE_E_EDIT_V3_FV(c->, "V", velocity);
		CE_E_EDIT_V3_FV(c->, "A", acceleration);
	});
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE