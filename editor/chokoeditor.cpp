#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

Background ss;

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color(0.5f));
	EWindowManager::Draw();
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

	assetPath = IO::path() + "project/assets/";
	EAssetList::Rescan();

	Scene::AddNewObject()
		->name("__Editor_Cameras__");

	ss = Background::New(IO::path() + "res/sky.hdr", 6);

	Scene::sky(ss);
	Scene::sky()->brightness(0);
	//*
	Scene::AddObject((SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/rb/rabbit house.blend/rabbit house.blend.prefab"));
	Scene::objects().back()->transform()->localPosition(Vec3(-1.2f, -1.5f, 2));
	Scene::objects().back()->transform()->localRotationEuler(Vec3(0, -5, 0));
	
	
	const auto& cl = Scene::FindByName("celing lamp");
	if (!!cl) {
		const auto& o = Scene::AddNewObject(cl);
		const auto& l = o->AddComponent<Light>(LightType::Point);
		l->distance(20);
		l->strength(3);
		l->shadow(true);
		l->shadowStrength(0.7f);
		l->shadowBias(0.002f);
		l->radius(0.2f);
		l->color(Color(1, 0.9f, 0.7f));
		l->softShadows(true);
		o->transform()->localPosition(Vec3(0, -0.7f, 0));
	}
	/*
	const auto& o2 = Scene::AddNewObject(Scene::FindByName("rabbit house pillar lamp.001"));
	const auto& l2 = o2->AddComponent<Light>(LightType::Point);
	l2->distance(10);
	l2->strength(0);
	//l2->shadow(true);
	l2->shadowStrength(0.5f);
	l2->shadowBias(0.01f);
	l2->color(Color(1, 0.7f, 0.3f));
	o2->transform()->localPosition(Vec3(0.4f, 0, 0));
	*/
	/*
	const auto& o3 = Scene::AddNewObject();
	o3->transform()->localPosition(Vec3(1, 0, 1));
	const auto& pr = o3->AddComponent<LightProbe>();
	pr->resolution(128);
	pr->updateFrequency(LightProbeUpdateFrequency::OnStart);
	pr->strength(0);
	const auto& mr3 = o3->AddComponent<MeshRenderer>();
	mr3->mesh(ProceduralMesh::UVSphere(20, 10, 1));
	const auto& mt3 = Material::New();
	mt3->shader((Shader)EAssetList::Get(EAssetType::Shader, ".internal/unlit_cubemap.shader"));
	mt3->SetUniform("map", pr->result());
	mr3->materials({mt3});
	*/

	EWindowManager::LoadWindows();

	std::cout << Scene::Tree() << std::endl;

	UIButtonStyle style(Color(0.1f, 1));
	style.textNormal(Color::white());

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			UI_Ext::PreLoop();
			EWindowManager::Update();
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_ED_NAMESPACE