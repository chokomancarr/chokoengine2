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
	EOverlayManager::Init();

	assetPath = IO::path() + "project/assets/";
	EAssetList::Rescan();

	scene = Scene::New();

	scene->AddNewObject()
		->name("__Editor_Cameras__");
	auto scenebase = scene->AddNewObject();
	scenebase->name("__scene__");

	//scene->sky(Background::New(IO::path() + "res/sky.hdr", 4, false));
	//scene->sky()->brightness(1);
	
	//auto obj = (SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/untitled.blend/untitled.blend.prefab", true);

	auto rb = PrefabManager::Instantiate((Prefab)EAssetList::Get(EAssetType::Prefab, ".exported/rb/rabbit house.blend/rabbit house.blend.prefab", true));
	scene->AddObject(rb, scenebase);
	rb->transform()->localPosition(Vec3(-1.2f, -1.5f, 2));
	rb->transform()->localRotationEuler(Vec3(0, -5, 0));

	const auto& oc = scene->AddNewObject(scenebase);
	oc->transform()->localPosition(Vec3(-3, 0.5f, 4));
	oc->transform()->localRotationEuler(Vec3(-5, 150, 0));
	oc->AddComponent<Camera>();

	//auto scr = rb->AddComponent<DummyScript>();
	//scr->name("Turner (Script)");
	//scr->info(EAssetList::GetScr("turner.hpp"));

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

	ESerializer::UpdateSceneIds();
	const pESerializedPrefab pfb(new ESerializedPrefab(scene->objects()[1]));
	JsonObject pfbj = pfb->ToJson();
	std::string pfbs = JsonParser::Export(pfbj);
	{
		std::ofstream pfbf(IO::path() + "a.json");
		pfbf << pfbs;
	}

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

	//EW_ShaderEditor::target = (VShader)EAssetList::Get(EAssetType::VShader, "test.visualshader");

	std::cout << scene->Tree() << std::endl;

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
