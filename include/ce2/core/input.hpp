#pragma once
#include "defines.hpp"
#include "enums/input_keys.hpp"

BEGIN_CE_NAMESPACE

class Input {

public:
	CE_OBJECT_COMMON;

	static void Init();

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

END_CE_NAMESPACE