#pragma once
#include "preferences.hpp"

CE_BEGIN_ED_NAMESPACE

class EPreferences::Input {
public:
	static float scrollLines;

	static void Init();

	static void Draw(UI_Ext::Layout::InfoSt&);
};

CE_END_ED_NAMESPACE