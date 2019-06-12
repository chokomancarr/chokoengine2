#include "chokolait.hpp"

namespace CE = ChokoEngine;

int ChokoLait::initd = 0;

CE::Camera ChokoLait::mainCamera;

void ChokoLait::_InitVars() {
	if (!CE::Engine::Init()) {
		exit(-1);
	}
}

ChokoLait::ChokoLait() {
	if (!initd) {
		_InitVars();
		initd = 1;
	}
}

void ChokoLait::Init(int scrW, int scrH) {
	if (!initd) {
		_InitVars();
		initd = 1;
	}

}

bool ChokoLait::alive() {
	return true;
}

void ChokoLait::Update(emptyCallbackFunc func) {

}

void ChokoLait::Paint(emptyCallbackFunc rendFunc, emptyCallbackFunc paintFunc) {

}