#include "chokolait.hpp"
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

bool ChokoLait::initd = false;

Scene ChokoLait::mainScene;

Camera ChokoLait::mainCamera;

bool ChokoLait::Init(const std::string& title, int w, int h) {
	if (initd) {
		Debug::Warning("ChokoLait", "Init called more than once!");
	}

	Display::Init();
	Display::InitWindow(title, w, h);

	if (!ChokoEngine::Init()) {
		return false;
	}

	if (!Backend::Renderer::Init() || !Backend::GI::Init()) {
		return false;
	}

	mainScene = Scene::New();

	initd = true;

	glClearColor(0, 0, 0, 0);
	glClearDepth(1);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//glFrontFace(GL_CW);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	Time::Reset();

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

	mainScene->Update();
}

void ChokoLait::Paint(emptyCallbackFunc rendFunc, emptyCallbackFunc paintFunc) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Backend::Renderer::Render(mainScene);

	if (paintFunc)
		paintFunc();

	glfwSwapBuffers(Display::_window);
}

CE_END_NAMESPACE
