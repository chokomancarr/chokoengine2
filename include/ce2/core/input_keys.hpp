#pragma once

BEGIN_CE_NAMESPACE
enum class InputKey {
    None = 0,
	Space = 32,
	Apostrophe = 39,
	Comma = 44, Minus, Dot, Slash,
	Num0 = 48, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
	Semicolon = 59, Equal = 61,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	LeftBracket = 91, BackSlash, RightBracket, Accent = 96,
	Escape = 256, Enter, Tab, Backspace, Insert, Delete,
	RightArrow, LeftArrow, DownArrow, UpArrow, PageUp, PageDown, Home, End, CapsLock, ScrollLock, NumLock,
	F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	NumPad0 = 320, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
	NumPadDot, NumPadDivide, NumPadMultiply, NumPadMinus, NumPadAdd, NumPadEnter, NumPadEqual,
	LeftShift = 340, LeftControl, LeftAlt, RightShift = 344, RightControl, RightAlt
};
END_CE_NAMESPACE
