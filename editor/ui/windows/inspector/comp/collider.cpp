#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/collider.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(Collider)
CE_E_EDIT_ENUM("Type", c->type, ColliderTypeStr, c);
switch (c->type()) {
case ColliderType::InfPlane:
	break;
case ColliderType::Sphere: {
	CE_E_EDIT_F_FV(c->, "Radius", radius);
	break;
}
default:
	break;
}
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE