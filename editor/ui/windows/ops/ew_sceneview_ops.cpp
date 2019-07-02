#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_IMPL_CALLBACK(EW_SceneView::Ops::ProjectionMode) {
    ((EW_SceneView*)window)->_camera->orthographic(!!args[0].i);
}

CE_END_ED_NAMESPACE