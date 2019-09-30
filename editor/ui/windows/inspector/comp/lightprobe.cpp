#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/lightprobe.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(LightProbe)
CE_E_EDIT_F_FV(c->, "Strength", strength);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE