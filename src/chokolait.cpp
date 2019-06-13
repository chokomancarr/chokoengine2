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
}

bool ChokoLait::alive() {
	glfwPollEvents();
	return !glfwWindowShouldClose(CE::Display::_window);
}

void ChokoLait::Update(emptyCallbackFunc func) {

}

void ChokoLait::Paint(emptyCallbackFunc rendFunc, emptyCallbackFunc paintFunc) {

}