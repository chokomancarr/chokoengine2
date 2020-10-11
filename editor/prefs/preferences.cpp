#include "preferences.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	typedef void(*drawFunc)(UI_Ext::Layout::InfoSt& lt);
	typedef std::pair<const std::string, const drawFunc> PT;
	std::array<PT, 2> drawFuncs = {
		PT{ "Input", EPreferences::Input::Draw },
		PT{ "Colors", EPreferences::Colors::Draw }
	};

	size_t drawi = 0;
}

bool EPreferences::showSettings = false;

void EPreferences::Init() {

}

void EPreferences::Draw() {
	UI::Rect(Display::fullscreenRect(), Color(0.1f, 0.7f));

	for (size_t a = 0; a < drawFuncs.size(); a++) {
		if (UI::I::Button(Rect(10, 10 + 17 * a, 150, 16), Color(0.3f, (a == drawi) ? 0.5f : 0.f)
				, drawFuncs[a].first) == InputMouseStatus::HoverUp) {
			drawi = a;
		}
	}

	UI_Ext::Layout::InfoSt lt = {};
	UI_Ext::Layout::BeginLayout(Display::fullscreenRect().sub(200, 10, 10, 10), lt);
	drawFuncs[drawi].second(lt);

	if (CE_NS Input::KeyUp(InputKey::Escape)) {
		showSettings = false;
	}
}

CE_END_ED_NAMESPACE