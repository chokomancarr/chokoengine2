#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::CaptureCallbacks::OnPostBlit() {
	const auto& mat = parent->_camera->lastViewProjectionMatrix();

	glDisable(GL_CULL_FACE);

    parent->DoDrawScene(ChokoEditor::scene->objects()[1]->children());

	if (!!ESceneInfo::selectedObject) {
		const auto& tr = ESceneInfo::selectedObject->transform();
		const auto& pos = tr->worldPosition();
		const auto& ptr = parent->_pivot->transform();

		for (auto& c : ESceneInfo::selectedObject->components()) {
			const auto& f = EW_S_DrawCompList::activeFuncs[(int)c->componentType];
			if (f) f(c, mat);
		}

		const float operatorSize = 200; //relative to pixels

		EW_S_Operator::Draw(
			Mat4x4::FromTRS(pos, tr->worldRotation(), Vec3(
				operatorSize / parent->position.h())),
			mat * ptr->worldMatrix() * Mat4x4::FromTRS(
				ptr->worldPosition(), ptr->worldRotation(), Vec3(1.f)).inverse()
		);
	}

	glEnable(GL_CULL_FACE);
}

CE_END_ED_NAMESPACE