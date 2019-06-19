#include "chokoeditor.hpp"
#include "parsers/json.hpp"
#include <chrono>

CE_BEGIN_ED_NAMESPACE

void paint() {
	static auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	static std::chrono::milliseconds mso;
	UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string((ms - mso).count()) + " ms", Color::white());
	mso = ms;
}

void ChokoEditor::Init() {

}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 800, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	EImages::Init();
	EWindowManager::Init();

	ChokoLait::scene()->sky(Texture::New(IO::path() + "res/sky.hdr"));
	ChokoLait::scene()->AddNewObject()
		->name("__Editor_Cameras__");
	ChokoLait::scene()->AddNewObject()->name("Player");

	EWindowManager::LoadWindows();

	UIButtonStyle style(Color(0.1f, 1));
	style.textNormal(Color::white());
	
	
	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint(0, paint);
	}
}

CE_END_ED_NAMESPACE