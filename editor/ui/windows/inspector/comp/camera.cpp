#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/camera.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Camera)
    CE_E_EDIT_F_RNG(c->, "FoV", fov);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE