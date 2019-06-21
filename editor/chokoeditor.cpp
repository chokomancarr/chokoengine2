#include "chokoeditor.hpp"
#include "parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

long long _ms;

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());
}

void ChokoEditor::Init() {
	
}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 1000, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	Mesh m = MeshLoader::LoadObj(IO::path() + "res/cube.obj");

	EImages::Init();
	EWindowManager::Init();
	ESerializer::Init();

	ChokoLait::scene()->sky(Texture::New(IO::path() + "res/sky.hdr"));
	ChokoLait::scene()->AddNewObject()
		->name("__Editor_Cameras__");
	ChokoLait::scene()->AddNewObject()->name("Player");

	EWindowManager::LoadWindows();

	UIButtonStyle style(Color(0.1f, 1));
	style.textNormal(Color::white());
	
	
	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			EWindowManager::Update();
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_ED_NAMESPACE