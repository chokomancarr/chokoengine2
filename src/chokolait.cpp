#include "chokolait.hpp"
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

bool ChokoLait::initd = false;

Scene ChokoLait::scene;
Camera ChokoLait::mainCamera;

bool ChokoLait::Init(int w, int h, const InitOptionsSt& opts) {
	if (initd) {
		Debug::Warning("ChokoLait", "Init called more than once!");
		return true;
	}

	Display::Init();
	if (!opts.visible) {
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	}
	Display::InitWindow(opts.title, w, h);

	if (!ChokoEngine::Init()) {
		return false;
	}

	if (!Backend::Renderer::Init() || !Backend::GI::Init()) {
		return false;
	}

	if (opts.createScene) {
		scene = Scene::New();
	}

	glClearColor(0, 0, 0, 0);
	glClearDepth(1);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//glFrontFace(GL_CW);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	Time::Reset();

	initd = true;

	return true;
}

bool ChokoLait::alive() {
	return !glfwWindowShouldClose(Display::_window);
}

void ChokoLait::Update(emptyCallbackFunc func) {
	Input::PostLoop();
	glfwPollEvents();
	Time::Update();
	Input::PreLoop();
	UI::I::PreLoop();

	if (func)
		func();

	if (!!scene) {
		scene->Update();
	}
}

void ChokoLait::Paint(emptyCallbackFunc rendFunc, emptyCallbackFunc paintFunc) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!!scene) {
		Backend::Renderer::Render(scene);
	}

	scene->Paint();

	if (paintFunc)
		paintFunc();

	glfwSwapBuffers(Display::_window);
}

CE_END_NAMESPACE
