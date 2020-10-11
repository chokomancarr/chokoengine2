#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::CaptureCallbacks::OnUpdate() {
	if (!EW_S_Operator::editing && Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::None) {
		if (Input::KeyUp(InputKey::G)) {
			EW_S_Operator::mode = EW_S_Operator::Mode::Translate;
		}
		if (Input::KeyUp(InputKey::R)) {
			EW_S_Operator::mode = EW_S_Operator::Mode::Rotate;
		}
		if (Input::KeyUp(InputKey::S)) {
			EW_S_Operator::mode = EW_S_Operator::Mode::Scale;
		}
	}
}

void EW_SceneView::CaptureCallbacks::OnPostBlit() {
	const auto& mat = parent->_camera->lastViewProjectionMatrix();

	glDisable(GL_CULL_FACE);

    parent->DoDrawScene(ChokoEditor::scene->objects()[1]->children());

	if (!!ESceneInfo::selectedObject) {
		const auto& tr = ESceneInfo::selectedObject->transform();
		const auto& pos = tr->worldPosition();

		for (auto& c : ESceneInfo::selectedObject->components()) {
			const auto& f = EW_S_DrawCompList::activeFuncs[(int)c->componentType];
			if (f) f(c, mat);
		}

		EW_S_Operator::Draw(tr, parent, (parent->controlMode == EW_SceneView::ControlMode::None));
	}

	glEnable(GL_CULL_FACE);
}

void EW_SceneView::CaptureCallbacks::OnPostRender() {
	
}

CE_END_ED_NAMESPACE