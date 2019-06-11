#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class Display {
	static uint _width, _height;
	static float _dpiScl;
    
	static GLFWwindow* _window;
public:
	CE_OBJECT_COMMON;

	GET_ST_MEMBER_FUNC(uint, width);
	GET_ST_MEMBER_FUNC(uint, height);

    /* Creates a OpenGL window with width \p w and height \p h
     */
    static void InitWindow(uint w, uint h);

	static void Resize(uint w, uint h, bool maximize = false);
};

END_CE_NAMESPACE