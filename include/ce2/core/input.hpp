#pragma once
#include "defines.hpp"
#include "enums/input_key.hpp"
#include "enums/input_mouse_button.hpp"
#include "enums/input_mouse_status.hpp"

CE_BEGIN_NAMESPACE

class Input { CE_CLASS_COMMON

	static CE_EXPORT Vec2 _mousePositionOld;
	static CE_EXPORT Vec2 _mousePosition;
	static CE_EXPORT Vec2 _mouseDownPosition;

	static std::array<bool, 5> _mouseButtonStatesOld;
	static std::array<bool, 5> _mouseButtonStates;

	static bool Init();

	static void PreLoop();
	static void PostLoop();

	static void _OnCursorMove(GLFWwindow*, double, double);
	static void _OnMouseClick(GLFWwindow*, int, int, int);
public:
	CE_GET_ST_MEMBER(mousePosition);
	CE_GET_ST_MEMBER(mouseDownPosition);
	static Vec2 mouseDelta();

	/* Returns the status of mouse button \p b
	 */
	static InputMouseStatus mouseStatus(InputMouseButton b);

	/* Checks if key \p c is pressed this frame while not in the previous frame
	*/
	static bool KeyDown(InputKey c);

	/* Checks if key \p c is held down for more than one frame
	*/
	static bool KeyHold(InputKey c);
	
	/* Checks if key \p c is released this frame while pressed in the previous frame
	*/
	static bool KeyUp(InputKey c);
};

CE_END_NAMESPACE