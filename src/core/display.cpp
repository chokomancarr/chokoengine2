#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

uint Display::_width, Display::_height;
float Display::_dpiScl = 1;

GLFWwindow* Display::_window = nullptr;

bool Display::Init() {
	if (!glfwInit()) {
		Debug::Error("System", "Fatal: Cannot init glfw!");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return true;
}

bool Display::InitWindow(uint w, uint h) {
	if (!(_window = glfwCreateWindow(w, h, "ChokoEngine App", NULL, NULL))) {
		Debug::Error("System", "Fatal: Cannot create glfw window!");
		return false;
	}
	glfwMakeContextCurrent(_window);
	glewExperimental = true;
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		Debug::Error("System", "Glew error: " + std::string((const char*)glewGetErrorString(GlewInitResult)));
		abort();
	}

	_width = w;
	_height = h;

	return true;
}

void Display::Resize(uint x, uint y, bool maximize) {
	glfwSetWindowSize(_window, (int)x, (int)y);
}

END_CE_NAMESPACE