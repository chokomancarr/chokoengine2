#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class Display { CE_CLASS_COMMON
	static uint _width, _height;
	static float _dpiScl;
    
	static GLFWwindow* _window;

	static bool Init();
public:

	GET_ST_MEMBER_FUNC(uint, width);
	GET_ST_MEMBER_FUNC(uint, height);

    /* Creates a OpenGL window with width \p w and height \p h
     */
    static bool InitWindow(uint w, uint h);

	static void Resize(uint w, uint h, bool maximize = false);
};

END_CE_NAMESPACE