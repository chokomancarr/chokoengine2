#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::DoDrawScene(const std::vector<SceneObject>& objs) {
	for (auto& o : objs) {
		if (o == ESceneInfo::selectedObject) continue;
		for (auto& c : o->components()) {
			const auto& f = EW_S_DrawCompList::funcs[(int)c->componentType];
			if (f) f(c);
		}
		DoDrawScene(o->children());
	}
}

void EW_SceneView::DrawMenu() {
	const Rect& r = Rect(position.x(), position.y() + 20, position.w(), position.h() - 20);
	glBlendFunc(GL_ONE, GL_ZERO);
	UI::Texture(r, static_cast<Texture>(_target));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (r.Contains(Input::mouseDownPosition())) {
		static float x = 0;
		static float y = 0;
		if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Hold) {
			x -= Input::mouseDelta().y;
			y -= Input::mouseDelta().x;
			_pivot->transform()->localRotationEuler(Vec3(x, y, 0));
		}
	}
}

bool EW_SceneView::_Init() {
	EW_S_Light::Init();
	EW_S_Rig::Init();

	return true;
}

EW_SceneView::EW_SceneView() : EWindow("Scene") {}

bool EW_SceneView::Init() {
	_target = RenderTarget::New((uint)position.w(), (uint)(position.h() - 20), true, true);

	_pivot = Scene::AddNewObject(Scene::objects()[0]);
	_pivot->name("SceneView Pivot");
	//_pivot->transform()->localRotationEuler(Vec3(-14, -34, 0));
	auto o = Scene::AddNewObject(_pivot);
	o->name("SceneView Camera");
	o->transform()->localPosition(Vec3(0, 0, 3));
	_camera = o->AddComponent<Camera>();
	_camera->clearColor(Color(0, 0));
	_camera->target(_target);
	auto cc = o->AddComponent<Ref<CaptureCallbacks>>();
	cc->parent = this;

	menus.push_back(EDropdownMenu("View"));
	auto op = EDropdownMenu("Switch ortho/persp");
	op.callback = ECallback(&Ops::ProjectionMode);
	menus.back().items.push_back(op);
	menus.push_back(EDropdownMenu("Object"));
	menus.back().enabled = false;

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
	/*
	if (Input::KeyDown(InputKey::F)) {
		auto args = ECallbackArgs();
		args.Add(ECallbackArg("isOrtho", (int)(!_camera->orthographic())));
		menus.back().items[0].callback(this, args);
	}
	*/
}

CE_END_ED_NAMESPACE