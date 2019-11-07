#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

inline void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, UIScaling::Crop, Color(0.5f));
	EWindowManager::Draw();

	EOverlayManager::Draw();

	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());
}

std::string ChokoEditor::assetPath;

void ChokoEditor::Init() {
	
}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 1000, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	EPaths::Init();
	EAssetList::Init();
	EImages::Init();
	EIcons::Init();
	EWindowManager::Init();
	ESerializer::Init();
	EOverlayManager::Init();

	assetPath = IO::path() + "project/assets/";
	EAssetList::Rescan();

	Scene::AddNewObject()
		->name("__Editor_Cameras__");

	//ss = Background::New(IO::path() + "res/sky.hdr", 6, false);

	//Scene::sky(ss);
	//Scene::sky()->brightness(1);
	
	Scene::AddObject(((Prefab)EAssetList::Get(EAssetType::Prefab, ".exported/cornell.blend/cornell.blend.prefab", true))->Instantiate());
	//Scene::objects().back()->transform()->localPosition(Vec3(-1.2f, -1.5f, 2));
	//Scene::objects().back()->transform()->localRotationEuler(Vec3(0, -5, 0));

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

	//EW_ShaderEditor::target = (VShader)EAssetList::Get(EAssetType::VShader, "test.visualshader");

	std::cout << Scene::Tree() << std::endl;

	UIButtonStyle style(Color(0.1f, 1));
	style.textNormal(Color::white());

	EDropdownMenu menu("asdf");
	menu.items = {
		EDropdownMenu("aaaa"),
		EDropdownMenu("bbbb"),
		EDropdownMenu("cccc"),
	};
	//EO_Dropdown::Reg(Vec2(10, 10), menu, true);
	Debug::Message("Editor", "Startup finished");

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			UI_Ext::PreLoop();
			EWindowManager::Update();
			EOverlayManager::Update();
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_ED_NAMESPACE