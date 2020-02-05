#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

uint Display::_width, Display::_height;
float Display::_dpiScl = 1;

GLFWwindow* Display::_window = nullptr;

namespace {
	std::string lastGlfwError = "Unknown error!";
	void glfw_error_callback(int, const char* str)
	{
		lastGlfwError = str;
	}

}

bool Display::Init() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		Debug::Error("System", "Fatal: Cannot init glfw: " + lastGlfwError);
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(0);
	return true;
}

bool Display::InitWindow(const std::string& title, uint w, uint h) {
	if (!(_window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL))) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		if (!(_window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL))) {
			Debug::Error("System", "Fatal: Cannot create glfw window: " + lastGlfwError);
			return false;
		}
		Debug::Warning("System", "Using OpenGL 3.3 context because 4.3 is not supported");
	}
	glfwMakeContextCurrent(_window);

#ifndef PLATFORM_MAC
	glewExperimental = true;
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		Debug::Error("System", "Glew error: " + std::string((const char*)glewGetErrorString(GlewInitResult)));
		abort();
	}
#endif

	_width = w;
	_height = h;

	glfwSetWindowSizeCallback(_window, OnWindowSizeChanged);

	return true;
}

void Display::OnWindowSizeChanged(GLFWwindow* window, int w, int h) {
	_width = w;
	_height = h;
}

Rect Display::fullscreenRect() {
	return Rect(0, 0, _width, _height);
}

void Display::Resize(uint x, uint y, bool maximize) {
	glfwSetWindowSize(_window, (int)x, (int)y);
}

CE_END_NAMESPACE