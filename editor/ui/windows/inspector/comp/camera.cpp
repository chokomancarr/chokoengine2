#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/camera.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Camera)
	CE_E_EDIT_TG_FV(c->, "Orthographic", orthographic);
	if (c->orthographic()) {
		CE_E_EDIT_F_FV(c->, "Size", orthoSize);
	}
	else {
		CE_E_EDIT_F_RNG_FV(c->, "FoV", fov, 1, 179);
	}
	CE_E_EDIT_F_RNG_FV(c->, "Near Clip", nearClip, 0.00001f, c->farClip());
	CE_E_EDIT_F_RNG_FV(c->, "Far Clip", farClip, c->nearClip(), 1000000);

	CE_E_EDIT_ENUM("Clear", c->clearType, CameraClearTypeStr, c);
	const auto& ct = c->clearType();
	if (((int)ct & (int)CameraClearType::Color) > 0) {
		CE_E_EDIT_CL_FV(c->, "  Color", clearColor);
	}
	if (((int)ct & (int)CameraClearType::Depth) > 0) {
		CE_E_EDIT_F_RNG_FV(c->, "  Depth", clearDepth, 0, 1);
	}
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE