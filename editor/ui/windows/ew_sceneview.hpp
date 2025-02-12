#pragma once
#include "chokoeditor.hpp"
#include "sceneview/resources.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_SceneView : public EWindow {
	RenderTarget _target;
	SceneObject _pivot;
	Camera _camera;

	ChokoEngine::objectid highlightId;

	enum class ControlMode {
		None,
		Rotate,
		Pan,
		Scale,
	} controlMode = ControlMode::None;

	void DoDrawScene(const std::vector<SceneObject>& objs);

protected:
	void DrawMenu() override;

	void DrawScene();

public:
	static bool _Init();

	EW_SceneView();

	bool Init() override;

	void Update() override;

	void ActiveUpdate() override;

	void Render() override;

	CE_GET_MEMBER(target);
	CE_GET_MEMBER(pivot);
	CE_GET_MEMBER(camera);

	class Ops;

	class CaptureCallbacks;
};

CE_END_ED_NAMESPACE

#include "ops/ew_sceneview_ops.hpp"

#include "sceneview/callback_capture.hpp"
#include "sceneview/draw_comp_list.hpp"