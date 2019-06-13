#include "chokolait.hpp"

namespace CE = ChokoEngine;

bool ChokoLait::initd = false;

CE::Camera ChokoLait::mainCamera;

ChokoLait::ChokoLait(int w, int h) {
	if (!initd) {

		CE::Display::Init();
		CE::Display::InitWindow(w, h);

		if (!CE::Engine::Init()) {
			exit(-1);
		}

		initd = true;
	}

	glClearColor(1, 0, 0, 0);
	glClearDepth(1);
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