#pragma once
#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

class EPreferences {
public:
	class Colors;
	class Input;
	class Keymap;

	static void Init();

	static bool showSettings;

	static void Draw();
};

CE_END_ED_NAMESPACE

#include "colors.hpp"
#include "input.hpp"