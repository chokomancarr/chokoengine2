#include "chokoeditor.hpp"
#include "parsers/json.hpp"

CE_BEGIN_ED_NAMESPACE

void ChokoEditor::Init() {

}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 800, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	EImages::Init();
	EWindowManager::Init();

	EWindowManager::LoadWindows();

	auto o = ChokoLait::scene()->AddNewObject();
		o->name("New Object 1");
		ChokoLait::scene()->AddNewObject(o)->name("foo");
		ChokoLait::scene()->AddNewObject(o)->name("bar");
	o = ChokoLait::scene()->AddNewObject();
		o->name("Main Camera");
		ChokoLait::scene()->AddNewObject(o)->name("asdf1234");
	ChokoLait::scene()->AddNewObject()->name("Player");

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint(0, []() {
			//UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.2f));
			UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
			EWindowManager::Draw();
			UI::Label(Rect(200, 50, 100, 20), "Hello World!!!!", Color::white());
		});
	}
}

CE_END_ED_NAMESPACE