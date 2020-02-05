#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::DoDrawScene(const std::vector<SceneObject>& objs) {
	return;
	for (auto& o : objs) {
		if (o == ESceneInfo::selectedObject) continue;
		for (auto& c : o->components()) {
			const auto& f = EW_S_DrawCompList::funcs[(int)c->componentType];
			if (f) f(c, _camera->lastViewProjectionMatrix());
		}
		DoDrawScene(o->children());
	}
}

void EW_SceneView::DrawMenu() {
	const Rect& r = Rect(position.x(), position.y() + 20, position.w(), position.h() - 20);
	glBlendFunc(GL_ONE, GL_ZERO);
	UI::Texture(r, _target);
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
	EW_S_Camera::Init();
	EW_S_Light::Init();
	EW_S_Rig::Init();

	return true;
}

EW_SceneView::EW_SceneView() : EWindow("Scene") {}

bool EW_SceneView::Init() {
	_target = RenderTarget::New((uint)position.w(), (uint)(position.h() - 20), true, true);

	_pivot = ChokoEditor::scene->AddNewObject(ChokoEditor::scene->objects()[0]);
	_pivot->name("SceneView Pivot");
	//_pivot->transform()->localRotationEuler(Vec3(-14, -34, 0));
	auto o = ChokoEditor::scene->AddNewObject(_pivot);
	o->name("SceneView Camera");
	o->transform()->localPosition(Vec3(0, 0, 3));
	o->transform()->localRotationEuler(Vec3(0, 180, 0));
	_camera = o->AddComponent<Camera>();
	_camera->clearColor(Color(0, 0));
	_camera->target(_target);
	auto cc = o->AddComponent<Ref<CaptureCallbacks>>();
	cc->parent = this;

	menus.push_back(EDropdownMenu("View"));
	auto op = EDropdownMenu("Switch ortho/persp");
	menus.back().items.push_back(op);
	menus.push_back(EDropdownMenu("Object"));
	menus.back().enabled = false;

	return true;
}

void EW_SceneView::Update() {
	auto sw = (uint)position.w(), sh = (uint)(position.h() - 20);

	if (sw != _target->width() || sh != _target->height()) {
		_target = RenderTarget::New(sw, sh, true, true);
		_camera->target(_target);
	}
}

void EW_SceneView::Render() {
	ChokoEditor::scene->RenderCameras({_camera});
}

CE_END_ED_NAMESPACE