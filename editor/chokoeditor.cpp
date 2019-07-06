#include "chokoeditor.hpp"
#include "parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

MeshRenderer mr;
Material ms[3];

Background ss[3];

Light lht;

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());

	//ChokoLait::scene()->sky()->brightness(UI::I::Slider(Rect(10, 190, 100, 20), Vec2(0, 3), ChokoLait::scene()->sky()->brightness(), Color::yellow()));

	//lht->radius(UI::I::Slider(Rect(10, 210, 100, 20), Vec2(0, 2), lht->radius(), Color::yellow()));

	static int um2 = 1;
	if(Input::KeyDown(InputKey::M)) {
		mr->materials({ ms[um2++] });
		if (um2 > 2)
			um2 = 0;
	}

	static int us2 = 1;
	if(Input::KeyDown(InputKey::B)) {
		ChokoLait::scene()->sky(ss[us2++]);
		if (us2 > 2)
			us2 = 0;
	}

	static std::string s = "Hello";
	s = UI::I::TextField(Rect(Display::width() - 200, Display::height() - 30, 150, 20), s, UITextFieldStyle(Color::blue()));

	UI::Texture(Rect(0, 0, 100, 100), (Texture)EAssetList::Get(EAssetType::Texture, "aa.jpg"));
}

std::string ChokoEditor::assetPath;

void ChokoEditor::Init() {
	
}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 1000, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	EAssetList::Init();
	EImages::Init();
	EWindowManager::Init();
	ESerializer::Init();

	assetPath = IO::path() + "project/assets/";
	EAssetList::Rescan();

	ChokoLait::scene()->AddNewObject()
		->name("__Editor_Cameras__");

	ss[0] = Background::New(IO::path() + "res/sky.hdr", 6);
	ss[1] = Background::New(IO::path() + "res/skyy.hdr", 6);
	ss[2] = Background::New(IO::path() + "res/skyyy.hdr", 6);

	auto& sc = ChokoLait::scene();

	//auto obj = Prefab::Load("test.prefab");
	sc->sky(ss[0]);
	sc->sky()->brightness(1);
	//sc->AddObject((SceneObject)EAssetList::Get(EAssetType::SceneObject, "test.prefab"));

	auto arm = (Armature)EAssetList::Get(EAssetType::Armature, "Armature.armature");

	EWindowManager::LoadWindows();

	std::cout << ChokoLait::scene()->Tree() << std::endl;

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