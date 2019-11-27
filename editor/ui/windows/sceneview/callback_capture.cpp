#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::CaptureCallbacks::OnPostBlit() {
    parent->DoDrawScene(ChokoEditor::scene->objects());

	if (!!ESceneInfo::selectedObject) {
		for (auto& c : ESceneInfo::selectedObject->components()) {
			const auto& f = EW_S_DrawCompList::funcs[(int)c->componentType];
			if (f) f(c);
		}

		const auto& tr = ESceneInfo::selectedObject->transform();
		const auto& pos = tr->worldPosition();
		const auto& mat = parent->_camera->lastViewProjectionMatrix();
		UI::W::matrix(mat);
		UI::W::Line(pos, pos + tr->right() * 0.3f, Color::red());
		UI::W::Line(pos, pos + tr->up() * 0.3f, Color::green());
		UI::W::Line(pos, pos + tr->forward() * 0.3f, Color::blue());
	}
}

CE_END_ED_NAMESPACE