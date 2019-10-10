#pragma once
#include "enums/input_key.hpp"
#include "enums/input_mouse_button.hpp"
#include "enums/input_mouse_status.hpp"

CE_BEGIN_NAMESPACE

class Input { CE_CLASS_COMMON

	static bool Init();

	static void PreLoop();
	static void PostLoop();

	static void _OnCursorMove(GLFWwindow*, double, double);
	static void _OnMouseClick(GLFWwindow*, int, int, int);
	static void _OnMouseScroll(GLFWwindow*, double, double);
	static void _OnKeyPress(GLFWwindow*, int, int, int, int);
	static void _OnCharInput(GLFWwindow*, uint);

public:
	class State {
	public:
		State();

		Vec2 mousePositionOld;
		Vec2 mousePosition;
		Vec2 mouseDownPosition;

		Vec2 mouseScroll;

		std::array<bool, 5> mouseButtonStatesOld;
		std::array<bool, 5> mouseButtonStates;

		std::array<bool, 350> keyStatesOld;
		std::array<bool, 350> keyStates;

		std::string inputString;
		std::u32string inputUnicodeString;
	};

private:
	static CE_EXPORT State _state;

public:
	CE_GET_SET_ST_MEMBER(state);

	static void Clear();

	/* Screen coordinates of the pointer
	 */
	static const Vec2& mousePosition();
	
	/* Screen coordinates where the mouse is last pressed
	 */
	static const Vec2& mouseDownPosition();

	/* Distance the pointer moved in screen coordinates
	 */
	static Vec2 mouseDelta();

	/* Distance the pointer scrolled
	 */
	static const Vec2& mouseScroll();

	/* Text input from the keyboard
	 * Use Key* functions for individual key inputs
	 */
	static const std::string& inputString();

	/* Text input from the keyboard as unicode codepoints
	 */
	static const std::u32string& inputUnicodeString();

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