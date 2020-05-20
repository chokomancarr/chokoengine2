#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#pragma optimize("", off)
void EW_SceneView::CaptureCallbacks::OnPostBlit() {
	const auto& mat = parent->_camera->lastViewProjectionMatrix();
	UI::W::matrix(mat);

	glDisable(GL_CULL_FACE);

    parent->DoDrawScene(ChokoEditor::scene->objects()[1]->children());

	if (!!ESceneInfo::selectedObject) {
		const auto& tr = ESceneInfo::selectedObject->transform();
		const auto& pos = tr->worldPosition();

		for (auto& c : ESceneInfo::selectedObject->components()) {
			const auto& f = EW_S_DrawCompList::activeFuncs[(int)c->componentType];
			if (f) f(c, mat);
		}

		UI::W::Line(pos, pos + tr->right() * 0.3f, Color::red());
		UI::W::Line(pos, pos + tr->up() * 0.3f, Color::green());
		UI::W::Line(pos, pos + tr->forward() * 0.3f, Color::blue());
	}

	glEnable(GL_CULL_FACE);
}

CE_END_ED_NAMESPACE