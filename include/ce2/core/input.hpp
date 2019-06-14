#pragma once
#include "defines.hpp"
#include "enums/input_keys.hpp"

CE_BEGIN_NAMESPACE

class Input { CE_CLASS_COMMON

	static void Init();
public:
	/* Checks if key \p c is pressed this frame but not pressed in the previous frame
	*/
	static bool KeyDown(InputKey c);

	/* Checks if key \p c is pressed and held from previous frames
	*/
	static bool KeyHold(InputKey c);
	
	/* Checks if key \p c is released this frame while pressed in the previous frame
	*/
	static bool KeyUp(InputKey c);
};

CE_END_NAMESPACE