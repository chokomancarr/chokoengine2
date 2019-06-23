#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::DrawMenu() {
	UI::Texture(Rect(position.x(), position.y() + 20, position.w(), position.h() - 20),
		static_cast<Texture>(_target));
}

bool EW_SceneView::Init() {
	_target = RenderTarget::New((uint)position.w(), (uint)(position.h() - 20), true);

	_pivot = ChokoLait::scene()->AddNewObject(ChokoLait::scene()->objects()[0]);
	_pivot->name("SceneView Pivot");
	auto o = ChokoLait::scene()->AddNewObject(_pivot);
	o->name("SceneView Camera");
	o->transform()->localPosition(Vec3(0, 0, -3));
	_camera = o->AddComponent<Camera>();
	_camera->clearColor(Color::black());
	_camera->target(_target);

	return true;
}

void EW_SceneView::Update() {
	_pivot->transform()->localRotation(
		Quat::FromEuler(Vec3(Math::Clamp(Input::mousePosition().y * 180.f / Display::height() - 90.f, -90.f, 90.f), 0, 0)) *
		Quat::FromEuler(Vec3(0, Input::mousePosition().x * 360.f / Display::width(), 0))
	);
}

CE_END_ED_NAMESPACE