#include "chokoeditor.hpp"
#include "parsers/json.hpp"

CE_BEGIN_ED_NAMESPACE

Font font;

void ChokoEditor::Init() {

}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 800, 600);

	font = Font::New(IO::path() + "res/font.ttf");
	font->size(50);

	EImages::Init();
	EWindowManager::Init();

	EWindowManager::LoadWindows();

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint(0, []() {
			UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.2f));
			UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
			EWindowManager::Draw();
			UI::Label(Rect(200, 50, 100, 20), "Hello World!!!!", Color::white(), font);
		});
	}
}

CE_END_ED_NAMESPACE