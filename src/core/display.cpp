#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

uint Display::_width = 256, Display::_height = 256;
float Display::_dpiScl = 1;

GLFWwindow* Display::_window = nullptr;

void Display::InitWindow(uint w, uint h) {

}

void Display::Resize(uint x, uint y, bool maximize) {
	glfwSetWindowSize(_window, (int)x, (int)y);
}

END_CE_NAMESPACE