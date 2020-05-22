#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_SceneView::Ops {

public:
	static void Reg();

    CE_DEF_CALLBACK(ToggleProjectionMode);

    CE_DEF_CALLBACK(ViewOrientation);

	CE_DEF_CALLBACK(ViewFocusObject);
};

CE_END_ED_NAMESPACE