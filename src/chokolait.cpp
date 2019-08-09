#include "chokolait.hpp"
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

bool ChokoLait::initd = false;

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

	if (!Backend::Renderer::Init()) {
		return false;
	}

	initd = true;

	glClearColor(0, 0, 0, 0);
	glClearDepth(1);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

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

	Scene::Update(Scene::_objects);
}

void ChokoLait::Paint(emptyCallbackFunc rendFunc, emptyCallbackFunc paintFunc) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Backend::Renderer::Render();

	if (paintFunc)
		paintFunc();

	glfwSwapBuffers(Display::_window);
}

CE_END_NAMESPACE