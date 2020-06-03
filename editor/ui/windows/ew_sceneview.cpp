#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_SceneView::DoDrawScene(const std::vector<SceneObject>& objs) {
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
}

bool EW_SceneView::_Init() {
	EW_S_Camera::Init();
	EW_S_Light::Init();
	EW_S_Rig::Init();

	Ops::Reg();

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

	//move this to a generic hotkey handler
	if (position.Contains(Input::mousePosition())) {
		if (Input::KeyUp(InputKey::F)) {
			ECallbackManager::Invoke(CallbackSig::VIEW_FOCUS_OBJECT, ECallbackArgs({
				ECallbackArg("obj", ESceneInfo::selectedObject)
				}), this);
		}
		if (Input::KeyUp(InputKey::P)) {
			ECallbackManager::Invoke(CallbackSig::VIEW_TOGGLE_PERSPECTIVE, ECallbackArgs(), this);
		}
	}
}

void EW_SceneView::ActiveUpdate() {
	switch (Input::mouseStatus(InputMouseButton::Left)) {
	case InputMouseStatus::Down: {
		if (Input::KeyHold(InputKey::LeftShift)) {
			controlMode = ControlMode::Pan;
		}
		else if (Input::KeyHold(InputKey::LeftControl)) {
			controlMode = ControlMode::Scale;
		}
		else {
			controlMode = ControlMode::Rotate;
		}
		break;
	}
	case InputMouseStatus::Up: {
		controlMode = ControlMode::None;
		break;
	}
	default:
		break;
	}

	static float s = 0;

	switch (controlMode) {
	case ControlMode::Rotate: {
		static float x = 0;
		static float y = 0;
		x -= Input::mouseDelta().y;
		y -= Input::mouseDelta().x;
		_pivot->transform()->localRotationEuler(Vec3(x, y, 0));
		break;
	}
	case ControlMode::Scale: {
		s -= 0.1f * Input::mouseDelta().y;
		break;
	}
	case ControlMode::Pan: {

		break;
	}
	default:
		break;
	}

	s -= 0.2f * Input::mouseScroll().y;
	_pivot->transform()->localScale(Vec3(std::pow(2, s)));
}

void EW_SceneView::Render() {
	ChokoEditor::scene->RenderCameras({_camera});
}

CE_END_ED_NAMESPACE