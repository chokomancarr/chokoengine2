#include "preferences.hpp"

CE_BEGIN_ED_NAMESPACE

std::map<std::string, std::vector<EPreferences::Colors::Entry>> EPreferences::Colors::entries = {};

void EPreferences::Colors::Init() {

}

void EPreferences::Colors::Reg(const std::string& grp, const std::string& sig, Color& tar) {
	entries[grp].push_back(Entry{ &tar, sig, tar });
}

void EPreferences::Colors::Draw(UI_Ext::Layout::InfoSt& lt) {

}

CE_END_ED_NAMESPACE