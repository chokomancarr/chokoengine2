#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::DrawMenu() {
	UI::Texture(Rect(position.x(), position.y() + 20, position.w(), position.h() - 20),
		static_cast<Texture>(_target));
}

bool EW_SceneView::Init() {
	_target = RenderTarget::New((uint)position.w(), (uint)(position.h() - 20), true, true);

	_pivot = Scene::AddNewObject(Scene::objects()[0]);
	_pivot->name("SceneView Pivot");
	auto o = Scene::AddNewObject(_pivot);
	o->name("SceneView Camera");
	o->transform()->localPosition(Vec3(0, 0, -3));
	_camera = o->AddComponent<Camera>();
	_camera->clearColor(Color(0));
	_camera->target(_target);

	menus.push_back(EDropdownMenu("View"));
	auto op = EDropdownMenu("Switch ortho/persp");
	op.callback = ECallback(&Ops::ProjectionMode);
	menus.back().items.push_back(op);
	menus.push_back(EDropdownMenu("Object"));

	return true;
}

void EW_SceneView::Update() {
	/*
	_pivot->transform()->localRotation(
		//Quat::FromEuler(Vec3(Math::Clamp(Input::mousePosition().y * 180.f / Display::height() - 90.f, -90.f, 90.f), 0, 0)) *
		Quat::FromEuler(Vec3(0, Time::time() * 15.f, 0))
	);
	static float z = -5;
	z = glm::clamp(z + Input::mouseDelta().y * 5 / Display::height(), -5.f, -2.f);
	_camera->object()->transform()->localPosition(Vec3(0, 0, z));
	*/
	if (Input::KeyDown(InputKey::F)) {
		auto args = ECallbackArgs();
		args.Add(ECallbackArg("isOrtho", (int)(!_camera->orthographic())));
		menus.back().items[0].callback(this, args);
	}
}

CE_END_ED_NAMESPACE