#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"
#include "ce2/modules/ae/asset_loader.hpp"

CE_BEGIN_ED_NAMESPACE

inline void paint() {
	ChokoEditor::scene->PrepareSceneForRendering();
	EWindowManager::Render();

	UI::Texture(Display::fullscreenRect(), EImages::background, UIScaling::Crop, Color(0.5f));
	EWindowManager::Draw();

	EOverlayManager::Draw();

	if (!EDragDrop::IsEmpty()) {
		Vec2 pos = Input::mousePosition() + Vec2(10, 5);
		for (auto& t : EDragDrop::target) {
			UI::Label(Rect(pos.x, pos.y, 1000, 20), t, Color(1, 0.8f));
			pos.y += 14;
		}
	}

	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());
}

std::string ChokoEditor::projectRoot;

Scene ChokoEditor::scene;

void ChokoEditor::Init() {
	_DummyScriptLoader::Init();
}

void ChokoEditor::Main() {
	ChokoLait::InitOptionsSt opts;
	opts.title = "ChokoEditor";
	ChokoLait::Init(1000, 600, opts);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	projectRoot = IO::path() + "project/";
	ModuleAE::AssetLoader::Init(CE_DIR_ASSET);

	EPaths::Init();
	EAssetList::Init();
	EImages::Init();
	EIcons::Init();
	EWindowManager::Init();
	EOverlayManager::Init();
	EScripting::Init();
	EPlayer::Init();

	EAssetList::Rescan();

	scene = Scene::New();

	ESceneManager::Init();

	EProjectBuilder::Init();

	ESceneManager::LoadLastOpened();

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

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
		EDragDrop::PostLoop();
	}
}

CE_END_ED_NAMESPACE
