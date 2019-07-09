#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::CaptureCallbacks::OnPostBlit() {
    parent->DoDrawScene(Scene::objects());
}

CE_END_ED_NAMESPACE