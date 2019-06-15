#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void ChokoEditor::Init() {

}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 800, 600);

	EImages::Init();

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint(0, []() {
			UI::Texture(Display::fullscreenRect(), EImages::background);
			UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
		});
	}
}

CE_END_ED_NAMESPACE