#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"
#include "utils/meshutils.hpp"

CE_BEGIN_ED_NAMESPACE

Background ss;
MeshSurfaceData dt;
Int2 sz;
Texture tx;
RenderTarget tx2t;
RenderTarget tx2;

inline void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, UIScaling::Crop, Color(0.5f));
	EWindowManager::Draw();

	EOverlayManager::Draw();

	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());

	glBlendFunc(GL_ONE, GL_ZERO);
	UI::Texture(Rect(10, Display::height() - 220, 200, 200), dt.GetInfoTex(sz).jmpInfoTex->tex(0));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UI::Texture(Rect(220, Display::height() - 220, 200, 200), (Texture)tx2);

	const Rect r3(Display::width() - 300, Display::height() - 320, 300, 300);

	glBlendFunc(GL_ONE, GL_ZERO);
	UI::Texture(r3, dt.GetInfoTex(sz).jmpInfoTex->tex(0));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UI::Texture(r3, (Texture)tx2);

	if (r3.Contains(Input::mousePosition())) {
		float mul = sz.x / r3.w();
		int px = (int)((Input::mousePosition().x - r3.x()) * mul);
		int py = sz.y - (int)((Input::mousePosition().y - r3.y()) * mul) - 1;

		auto& fb1 = dt.GetInfoTex(sz).uvInfoTex;
		fb1->Bind(true);
		Int4 vi;
		glReadPixels(px, py, 1, 1, GL_RGBA_INTEGER, GL_INT, &vi);
		fb1->Unbind(true);

		auto& fb2 = dt.GetInfoTex(sz).jmpInfoTex;
		fb2->Bind(true);
		Vec4 vj;
		glReadPixels(px, py, 1, 1, GL_RGBA, GL_FLOAT, &vj);
		fb2->Unbind(true);

		tx2->BindTarget(true);
		float vc[4];
		glReadPixels(px, py, 1, 1, GL_RGBA, GL_FLOAT, vc);
		tx2->UnbindTarget(true);

		UI::Label(Rect(r3.x(), r3.y() - 60, 200, 20), "ID: "
			+ std::to_string(vi.x) + ", "
			+ std::to_string(vi.y) + ", "
			+ std::to_string(vi.z) + ", "
			+ std::to_string(vi.w), Color::white());
		UI::Label(Rect(r3.x(), r3.y() - 40, 200, 20), "Jump: "
			+ std::to_string(vj.x) + ", "
			+ std::to_string(vj.y) + ", "
			+ std::to_string(vj.z) + ", "
			+ std::to_string(vj.w), Color::white());
		UI::Label(Rect(r3.x(), r3.y() - 20, 200, 20), "Result: "
			+ std::to_string(vc[0]) + ", "
			+ std::to_string(vc[1]) + ", "
			+ std::to_string(vc[2]) + ", "
			+ std::to_string(vc[3]), Color::white());
	}

	MeshUtils::SurfaceBlur(dt, tx, tx2, tx2t, 1);
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
	//auto obj = (SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/untitled.blend/untitled.blend.prefab", true);
	tx = (Texture)EAssetList::Get(EAssetType::Texture, "t.png");
	sz = Int2(tx->width(), tx->height());

	TextureOptions opts = TextureOptions(TextureWrap::Clamp, TextureWrap::Clamp, 0, false);
	tx2t = RenderTarget::New(sz.x, sz.y, true, false, opts);
	tx2 = RenderTarget::New(sz.x, sz.y, true, false, opts);
	
	auto obj = (SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/a.blend/a.blend.prefab", true);
	Scene::AddObject(obj);
	auto mr = obj->children()[0]->GetComponent<MeshRenderer>();
	dt = MeshUtils::GenSurfaceData(mr->mesh());
	dt.GenInfoTex(sz);

	mr->materials({
		(Material)EAssetList::Get(EAssetType::Material, "unlit.material")
	});
	mr->materials()[0]->SetUniform("tex", (Texture)tx2);

#if 0
	//Scene::AddObject((SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/cornell.blend/cornell.blend.prefab"));
	//*
	/*
	Scene::AddObject((SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/rb/rabbit house.blend/rabbit house.blend.prefab", true));
	Scene::objects().back()->transform()->localPosition(Vec3(-1.2f, -1.5f, 2));
	Scene::objects().back()->transform()->localRotationEuler(Vec3(0, -5, 0));
	
	auto scr = Scene::objects().back()->AddComponent<DummyScript>();
	scr->name("Turner (Script)");
	scr->info(EAssetList::GetScr("turner.hpp"));
	
	const auto& cl = Scene::FindByName("celing lamp");
	if (!!cl) {
		const auto& o = Scene::AddNewObject(cl);
		const auto& l = o->AddComponent<Light>(LightType::Point);
		l->distance(20);
		l->strength(3);
		l->shadow(true);
		l->shadowStrength(0.7f);
		l->shadowBias(0.002f);
		l->radius(0.05f);
		l->color(Color(1, 0.9f, 0.7f));
		l->softShadows(true);
		l->shadowSamples(20);
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
	const auto& mr3 = o3->AddComponent<MeshRenderer>();
	mr3->mesh(ProceduralMesh::UVSphere(20, 10, 1));
	const auto& mt3 = Material::New();
	mt3->shader((Shader)EAssetList::Get(EAssetType::Shader, ".internal/standard_transparent_color.shader"));
	mt3->SetUniform("color", Color(1, 1, 0.3f, 0.5f));
	mt3->SetUniform("ior", 1.2f);
	mt3->SetUniform("roughness", 0.3f);
	mr3->materials({mt3});
	/*
	const auto& pr = o3->AddComponent<LightProbe>();
	pr->resolution(128);
	pr->updateFrequency(LightProbeUpdateFrequency::OnStart);
	pr->strength(1);
	*/
#endif

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