#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::Ops::Reg() {
	ECallbackManager::Register(CallbackSig::VIEW_TOGGLE_PERSPECTIVE, ToggleProjectionMode);
	ECallbackManager::Register(CallbackSig::VIEW_FOCUS_OBJECT, ViewFocusObject);
}

CE_IMPL_CALLBACK(EW_SceneView::Ops::ToggleProjectionMode) {
	const auto& c = ((EW_SceneView*)window)->_camera;
	c->orthographic(!c->orthographic());
}

CE_IMPL_CALLBACK(EW_SceneView::Ops::ViewFocusObject) {
	auto o = args.Get<SceneObject>("obj");
	if (!o) return;
	((EW_SceneView*)window)->_pivot->transform()
		->localPosition(o->transform()->worldPosition());
}

CE_END_ED_NAMESPACE