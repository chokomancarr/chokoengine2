#include "chokolait.hpp"
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

bool ChokoLait::initd = false;

Scene ChokoLait::_scene;

Camera ChokoLait::mainCamera;

bool ChokoLait::Init(const std::string& title, int w, int h) {
	if (initd) {
		Debug::Warning("ChokoLait", "Init called more than once!");
	}

	Display::Init();
	Display::InitWindow(title, w, h);

	if (!Engine::Init()) {
		return false;
	}

	initd = true;

	glClearColor(1, 0, 0, 0);
	glClearDepth(1);

	_scene = Scene::New("New Scene");

	return true;
}

bool ChokoLait::alive() {
	glfwPollEvents();
	return !glfwWindowShouldClose(Display::_window);
}

void ChokoLait::Update(emptyCallbackFunc func) {

}

void ChokoLait::Paint(emptyCallbackFunc rendFunc, emptyCallbackFunc paintFunc) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Backend::Renderer::Render(_scene);

	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (paintFunc)
		paintFunc();

	glfwSwapBuffers(Display::_window);
}

CE_END_NAMESPACE