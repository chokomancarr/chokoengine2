#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/light.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Light)
CE_E_EDIT_F_FV(c->, "Strength", strength);
CE_E_EDIT_F_FV(c->, "Distance", distance);
CE_E_EDIT_F_FV(c->, "Radius", radius);
CE_E_EDIT_F_FV(c->, "Angle", angle);
CE_E_EDIT_TG_FV(c->, "Shadows", shadow);
CE_E_EDIT_I_FV(c->, " Resolution", shadowResolution);
CE_E_EDIT_F_FV(c->, " Strength", shadowStrength);
CE_E_EDIT_F_FV(c->, " Bias", shadowBias);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE