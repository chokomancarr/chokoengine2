#pragma once
#include "chokoeditor.hpp"

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
};

CE_END_ED_NAMESPACE