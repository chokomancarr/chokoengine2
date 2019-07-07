#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_SceneView : public EWindow {
	RenderTarget _target;
	SceneObject _pivot;
	Camera _camera;

	void DoDrawScene(const std::vector<SceneObject>& objs);

protected:
	void DrawMenu() override;

	void DrawScene();

public:
	static bool _Init();

	bool Init() override;

	void Update() override;

	class Ops;
};

CE_END_ED_NAMESPACE

#include "ops/ew_sceneview_ops.hpp"

#include "sceneview/draw_comp_list.hpp"