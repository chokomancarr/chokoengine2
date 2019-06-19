#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_SceneView : public EWindow {
	RenderTarget _target;
	SceneObject _pivot;
	Camera _camera;

protected:
	void DrawMenu() override;

public:
	bool Init() override;

	void Update() override;
};

CE_END_ED_NAMESPACE