#include "chokoeditor.hpp"
#include "glsl/minVert.h"
#include "glsl/outlineFrag.h"
#include "utils/glutils.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	bool selectobjectid(const ChokoEngine::objectid id, const std::vector<SceneObject>& oo = ChokoEditor::scene->objects()) {
		for (auto& o : oo) {
			if (o->id() == id) {
				ESceneInfo::Select(o);
				return true;
			}
			else {
				if (selectobjectid(id, o->children()))
					return true;
			}
		}
		return false;
	}

	Shader hlShad;

	Color hlCol = Color(0.5f);
}

void EW_SceneView::DoDrawScene(const std::vector<SceneObject>& objs) {
	if (highlightId != 0) {
		hlShad->Bind();
		glUniform2f(hlShad->Loc(0), 1.f / _target->width(), 1.f / _target->height());
		glUniform1i(hlShad->Loc(1), 0);
		glActiveTexture(GL_TEXTURE0);
		_camera->deferredBuffer()->tex(4)->Bind();
		glUniform1i(hlShad->Loc(2), highlightId);
		glUniform1i(hlShad->Loc(3), 1);
		glUniform4f(hlShad->Loc(4), hlCol.r, hlCol.g, hlCol.b, hlCol.a);
		GLUtils::DrawArrays(GL_TRIANGLES, 6);
		hlShad->Unbind();
	}

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
	EW_S_Resources::Init();
	EW_S_Rig::Init();
	EW_S_Operator::Init();

	Ops::Reg();

	(hlShad = Shader::New(glsl::minVert, glsl::outlineFrag))
		->AddUniforms({
			"dscreenSize", "idtex", "tarid",
			"radius", "color"
		});

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
	_camera->writeExtraBuffers(true);
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

	highlightId = 0;
}

void EW_SceneView::ActiveUpdate() {
	if (EW_S_Operator::editing) {
		if (EW_S_Operator::editingParent == this) {
			EW_S_Operator::Update();
		}
		return;
	}

	bool onclick = false;

	switch (Input::mouseStatus(InputMouseButton::Left)) {
	case InputMouseStatus::Hold: {
		if (controlMode != ControlMode::None
			|| Input::mouseDelta() == Vec2()) break;
		if (Input::KeyHold(InputKey::LeftShift)) {
			controlMode = ControlMode::Pan;
		}
		else if (Input::KeyHold(InputKey::LeftControl)) {
			controlMode = ControlMode::Scale;
		}
		else {
			controlMode = ControlMode::Rotate;
		}
		Cursor::locked(true);
		break;
	}
	case InputMouseStatus::Up: {
		onclick = (controlMode == ControlMode::None);
		controlMode = ControlMode::None;
		Cursor::locked(false);
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
		x += Input::mouseDelta().y;
		y += Input::mouseDelta().x;
		_pivot->transform()->localRotationEuler(Vec3(x, y, 0));
		break;
	}
	case ControlMode::Scale: {
		s += 0.05f * Input::mouseDelta().x;
		_pivot->transform()->localScale(Vec3(std::powf(2, s)));
		break;
	}
	case ControlMode::Pan: {
		if (_camera->orthographic()) {
			CE_NOT_IMPLEMENTED
		}
		else {
			const auto& ctr = _camera->object()->transform();
			float z = ctr->localPosition().z
				* _pivot->transform()->localScale().x;
			float th = _camera->fov() / 2;
			float dy = std::tan(th * Math::deg2rad) * z;
			float dx = dy * _target->width() / _target->height();
			float mdx = Input::mouseDelta().x / position.w() * 2;
			float mdy = Input::mouseDelta().y / position.h() * 2;
			_pivot->transform()->localPosition(
				_pivot->transform()->localPosition()
				+ ctr->right() * dx * mdx
				+ ctr->up() * dy * mdy
			);
		}
		
		break;
	}
	default:
		s -= 0.2f * Input::mouseScroll().y;
		_pivot->transform()->localScale(Vec3(std::powf(2, s)));

		const auto& gbuf = _camera->deferredBuffer();
		if (gbuf) {
			auto pos = Input::mousePosition() - Vec2(position.x(), position.y());
			pos.y = position.h() - pos.y;
			gbuf->Bind(true);
			glReadBuffer(GL_COLOR_ATTACHMENT4);
			int id[4] = {};
			glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA_INTEGER, GL_INT, id);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

			highlightId = id[0];
			if (onclick) {
				if (highlightId != 0) {
					selectobjectid(highlightId);
				}
				else {
					if (!!ESceneInfo::selectedObject) {
						ESceneInfo::Clear();
					}
				}
			}
		}

		break;
	}
}

void EW_SceneView::Render() {
	ChokoEditor::scene->RenderCameras({_camera});
}

CE_END_ED_NAMESPACE