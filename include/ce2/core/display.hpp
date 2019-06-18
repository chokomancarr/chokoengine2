#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Display { CE_CLASS_COMMON
	static CE_EXPORT uint _width, _height;
	static CE_EXPORT float _dpiScl;
    
	static CE_EXPORT GLFWwindow* _window;

	static bool Init();
	static bool InitWindow(const std::string& title, uint w, uint h);
public:

	CE_GET_ST_MEMBER(width);
	CE_GET_ST_MEMBER(height);

	static Rect fullscreenRect();

	static void Resize(uint w, uint h, bool maximize = false);

	friend class Input;
};

CE_END_NAMESPACE