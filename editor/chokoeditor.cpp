#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

inline void paint() {
	ChokoEditor::scene->PrepareSceneForRendering();
	EWindowManager::Render();

	UI::Texture(Display::fullscreenRect(), EImages::background, UIScaling::Crop, Color(0.5f));
	EWindowManager::Draw();

	EOverlayManager::Draw();

	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());
}

std::string ChokoEditor::assetPath;

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

	EPaths::Init();
	EAssetList::Init();
	EImages::Init();
	EIcons::Init();
	EWindowManager::Init();
	EOverlayManager::Init();
	EScripting::Init();

	assetPath = IO::path() + "project/assets/";
	EAssetList::Rescan();

	scene = Scene::New();

	ESceneManager::Init();

	ESceneManager::Load("tes.scene");
	/*
	scene->objects()[1]->children()[1]->AddComponent<DummyScript>(
		EAssetList::GetScr("turner.hpp")
	);
	*/
	ESceneManager::Save("tess.scene");

	//scene->sky((Background)EAssetList::Get(AssetType::Background, "sky2.hdr"));
	/*
	auto rb = ((Prefab)EAssetList::Get(AssetType::Prefab,
		".exported/rb/rabbit house.blend/rabbit house.blend.prefab")
		)->Instantiate([](AssetType t, const std::string& s) -> Asset {
		return EAssetList::Get(t, s, true);
	});
	scene->AddObject(rb, scene->objects()[1]);

	rb->transform()->localPosition(Vec3(-1.2f, -1.5f, 2));
	rb->transform()->localRotationEuler(Vec3(0, -5, 0));

	const auto& oc = scene->AddNewObject(scene->objects()[1]);
	oc->transform()->localPosition(Vec3(-3, 0.5f, 4));
	oc->transform()->localRotationEuler(Vec3(-5, 150, 0));
	oc->AddComponent<Camera>();

	const auto& cl = scene->FindByName("celing lamp");
	if (!!cl) {
		const auto& o = scene->AddNewObject(cl);
		const auto& l = o->AddComponent<Light>(LightType::Point);
		l->distance(20);
		l->strength(3);
		l->shadow(true);
		l->shadowStrength(0.7f);
		l->shadowBias(0.002f);
		l->radius(0.05f);
		l->color(Color(1, 0.9f, 0.7f));
		l->softShadows(true);
		l->shadowSamples(1);
		o->transform()->localPosition(Vec3(0, -0.7f, 0));
	}
	*/

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

	//std::cout << scene->Tree() << std::endl;

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
