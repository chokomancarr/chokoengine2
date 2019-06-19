#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::DrawMenu() {
	UI::Texture(Rect(position.x() + 1, position.y() + 21, position.w() - 2, position.h() - 22),
		static_cast<Texture>(_target));
}

bool EW_SceneView::Init() {
	_target = RenderTarget::New((uint)(position.w() - 2), (uint)(position.h() - 22), true);

	auto o = ChokoLait::scene()->AddNewObject(ChokoLait::scene()->objects()[0]);
	o->name("SceneView Camera");
	_camera = o->AddComponent<Camera>();
	_camera->clearColor(Color::blue());
	_camera->target(_target);
	return true;
}

CE_END_ED_NAMESPACE