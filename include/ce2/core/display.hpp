#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Display { CE_CLASS_COMMON
	static uint _width, _height;
	static float _dpiScl;
    
	static GLFWwindow* _window;

	static bool Init();
	static bool InitWindow(uint w, uint h);
public:

	CE_GET_ST_MEMBER(width);
	CE_GET_ST_MEMBER(height);

	static void Resize(uint w, uint h, bool maximize = false);
};

CE_END_NAMESPACE