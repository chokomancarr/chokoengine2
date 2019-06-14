#include "chokolait.hpp"

namespace CE = ChokoEngine;

bool ChokoLait::initd = false;

CE::Camera ChokoLait::mainCamera;

bool ChokoLait::Init(const std::string& title, int w, int h) {
	if (initd) {
		CE::Debug::Warning("ChokoLait", "Init called more than once!");
	}

	CE::Display::Init();
	CE::Display::InitWindow(title, w, h);

	if (!CE::Engine::Init()) {
		return false;
	}

	initd = true;

	glClearColor(1, 0, 0, 0);
	glClearDepth(1);

	return true;
}

bool ChokoLait::alive() {
	glfwPollEvents();
	return !glfwWindowShouldClose(CE::Display::_window);
}

void ChokoLait::Update(emptyCallbackFunc func) {

}

void ChokoLait::Paint(emptyCallbackFunc rendFunc, emptyCallbackFunc paintFunc) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (paintFunc)
		paintFunc();

	glfwSwapBuffers(CE::Display::_window);
}