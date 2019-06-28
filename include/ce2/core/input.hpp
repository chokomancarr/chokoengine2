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

	static std::array<bool, 350> _keyStatesOld;
	static std::array<bool, 350> _keyStates;

	static std::string _inputString;
	static std::u32string _inputUnicodeString;

	static bool Init();

	static void PreLoop();
	static void PostLoop();

	static void _OnCursorMove(GLFWwindow*, double, double);
	static void _OnMouseClick(GLFWwindow*, int, int, int);
	static void _OnKeyPress(GLFWwindow*, int, int, int, int);
	static void _OnCharInput(GLFWwindow*, uint);
public:
	/* Screen coordinates of the pointer
	 */
	CE_GET_ST_MEMBER(mousePosition);
	
	/* Screen coordinates where the mouse is last pressed
	 */
	CE_GET_ST_MEMBER(mouseDownPosition);

	/* Distance the pointer moved in screen coordinates
	 */
	static Vec2 mouseDelta();

	/* Text input from the keyboard
	 * Use Key* functions for individual key inputs
	 */
	CE_GET_ST_MEMBER(inputString);

	/* Text input from the keyboard as unicode codepoints
	 */
	CE_GET_ST_MEMBER(inputUnicodeString);

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