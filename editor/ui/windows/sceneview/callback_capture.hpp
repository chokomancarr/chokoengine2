#pragma once
#include "chokoeditor.hpp"
#include "operator.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_SceneView::CaptureCallbacks : public _Script {
public:
    EW_SceneView* parent;

	void OnUpdate() override;

    void OnPostBlit() override;

	void OnPostRender() override;
};

CE_END_ED_NAMESPACE