#pragma once
#include "preferences.hpp"

CE_BEGIN_ED_NAMESPACE

class EPreferences::Colors {
public:
	class Entry {
	public:
		Color* target;
		std::string sig;
		Color defVal;
	};

	static std::map<std::string, std::vector<Entry>> entries;

	static void Init();

	static void Reg(const std::string& grp, const std::string& sig, Color& tar);

	static void Draw(UI_Ext::Layout::InfoSt&);
};

CE_END_ED_NAMESPACE