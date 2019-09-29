#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

MeshRenderer mr;
Material ms[3];

Background ss[3];

Light lht;

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());

	//Scene::sky()->brightness(UI::I::Slider(Rect(10, 190, 100, 20), Vec2(0, 3), Scene::sky()->brightness(), Color::yellow()));

	//lht->radius(UI::I::Slider(Rect(10, 210, 100, 20), Vec2(0, 2), lht->radius(), Color::yellow()));

	// static std::string s = "Hello";
	// s = UI::I::TextField(Rect(Display::width() - 200, Display::height() - 30, 150, 20), s, UITextFieldStyle(Color::blue()));

	// UI::Texture(Rect(0, 0, 100, 100), (Texture)EAssetList::Get(EAssetType::Texture, "aa.jpg"));
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
	EIcons::Init();
	EWindowManager::Init();
	ESerializer::Init();

	assetPath = IO::path() + "project/assets/";
	EAssetList::Rescan();

	Scene::AddNewObject()
		->name("__Editor_Cameras__");

	ss[0] = Background::New(IO::path() + "res/sky.hdr", 6);
	ss[1] = Background::New(IO::path() + "res/skyy.hdr", 6);
	ss[2] = Background::New(IO::path() + "res/skyyy.hdr", 6);

	Scene::sky(ss[0]);
	Scene::sky()->brightness(0);
	//*
	Scene::AddObject((SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/rb/rabbit house.blend/rabbit house.blend.prefab"));
	Scene::objects().back()->transform()->localPosition(Vec3(-1.2f, -1.5f, 2));
	Scene::objects().back()->transform()->localRotationEuler(Vec3(0, -5, 0));
	
	const auto& cl = Scene::FindByName("celing lamp");
	if (!!cl) {
		auto& o = Scene::AddNewObject(cl);
		auto& l = o->AddComponent<Light>(LightType::Point);
		l->distance(20);
		l->strength(1);
		l->shadow(true);
		l->shadowBias(0.01f);
		l->color(Color(1, 0.9f, 0.7f));
		o->transform()->localPosition(Vec3(0, -0.7f, 0));
	}
	/*
	auto& o = Scene::AddNewObject(Scene::FindByName("celing lamp"));
	auto& l = o->AddComponent<Light>(LightType::Spot);
	l->distance(20);
	l->strength(1);
	l->color(Color(1, 0.9f, 0.7f));
	l->shadow(true);
	l->shadowBias(0.01f);
	o->transform()->localPosition(Vec3(0, -0.7f, 0));
	o->transform()->localRotationEuler(Vec3(90, 0, 0));
	*/
	auto& o2 = Scene::AddNewObject(Scene::FindByName("rabbit house pillar lamp.001"));
	auto& l2 = o2->AddComponent<Light>(LightType::Point);
	l2->distance(10);
	l2->strength(0.5f);
	l2->color(Color(1, 0.7f, 0.3f));
	o2->transform()->localPosition(Vec3(0.4f, 0, 0));
	
	const auto& cmap = CubeMap::New(256, GL_RGBA, TextureOptions(), 0);
	const auto& o3 = Scene::AddNewObject();
	const auto& mr3 = o3->AddComponent<MeshRenderer>();
	mr3->mesh(ProceduralMesh::UVSphere(20, 10, 1));
	//const auto& mt3 = Material::New();
	//mt3->shader((Shader)EAssetList::Get(EAssetType::Shader, ".internal/unlit_cubemap.shader"));
	//mt3->SetUniform("map", cmap);
	//mr3->materials({mt3});
	mr3->materials({ (Material)EAssetList::Get(EAssetType::Material, "def.material") });
	o3->AddComponent<LightProbe>();

	EWindowManager::LoadWindows();

	std::cout << Scene::Tree() << std::endl;

	UIButtonStyle style(Color(0.1f, 1));
	style.textNormal(Color::white());

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			EWindowManager::Update();
			//Scene::FindByName("body1")->transform()->localRotation(Quat::FromEuler(Vec3(0, std::cos(Time::time() * 2) * 10, 0)));
			//Scene::FindByName("head")->transform()->localRotation(Quat::FromEuler(Vec3(0, std::sin(Time::time() * 2) * 20, 0)));
			//static_cast<MeshShapeModifier>(Scene::FindByName("body")->GetComponent<MeshRenderer>()->modifiers()[0])->SetWeight(2, 1);
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_ED_NAMESPACE