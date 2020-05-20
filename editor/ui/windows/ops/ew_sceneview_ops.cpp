#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::Ops::Reg() {
	ECallbackManager::Register(CallbackSig::VIEW_FOCUS_OBJECT, ViewFocusObject);
}

CE_IMPL_CALLBACK(EW_SceneView::Ops::ProjectionMode) {
	
}

CE_IMPL_CALLBACK(EW_SceneView::Ops::ViewFocusObject) {
	((EW_SceneView*)window)->_pivot->transform()
		->localPosition(args.Get<SceneObject>("obj")->transform()->worldPosition());
}

CE_END_ED_NAMESPACE