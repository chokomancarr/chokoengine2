#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_SceneView::Ops {

public:
    CE_DEF_CALLBACK(ProjectionMode);

    CE_DEF_CALLBACK(ViewOrientation);
};

CE_END_ED_NAMESPACE