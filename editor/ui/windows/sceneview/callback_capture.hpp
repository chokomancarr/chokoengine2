#pragma once
#include "chokoeditor.hpp"
#include "operator.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_SceneView::CaptureCallbacks : public _Script {
public:
    EW_SceneView* parent;

    void OnPostBlit() override;
};

CE_END_ED_NAMESPACE