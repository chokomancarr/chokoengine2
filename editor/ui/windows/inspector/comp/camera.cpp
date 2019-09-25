#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/camera.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Camera)
    CE_E_EDIT_F_RNG_FV(c->, "FoV", fov, 1, 179);
	CE_E_EDIT_F_RNG_FV(c->, "Near Clip", nearClip, 0.00001f, c->farClip());
	CE_E_EDIT_F_RNG_FV(c->, "Far Clip", farClip, c->nearClip(), 1000000);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE