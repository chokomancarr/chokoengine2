#include "chokoeditor.hpp"
#include "parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

MeshRenderer mr;
Material ms[3];

Background ss[3];

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());

	ChokoLait::scene()->sky()->brightness(UI::I::Slider(Rect(10, 190, 100, 20), Vec2(0, 3), ChokoLait::scene()->sky()->brightness(), Color::yellow()));

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
}

void ChokoEditor::Init() {
	
}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 1000, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	Shader shd = Shader::New(
		IO::ReadFile(IO::path() + "res/shaders/standard_metallic.vs"),
		IO::ReadFile(IO::path() + "res/shaders/standard_metallic.fs"));
	shd->AddUniform("_MV", ShaderVariableType::Matrix);
	shd->AddUniform("_P", ShaderVariableType::Matrix);
	shd->AddUniform("_MVP", ShaderVariableType::Matrix);
	shd->AddUniform("col", ShaderVariableType::Vec4);
	shd->AddUniform("diffuseTex", ShaderVariableType::Texture);
	shd->AddUniform("normalTex", ShaderVariableType::Texture);
	shd->AddUniform("metalTex", ShaderVariableType::Texture);
	shd->AddUniform("roughTex", ShaderVariableType::Texture);
	shd->AddUniform("occluTex", ShaderVariableType::Texture);
	shd->AddUniform("normalness", ShaderVariableType::Float);

	TextureOptions opts(TextureWrap::Repeat, TextureWrap::Repeat, 5, true);

	std::string tp[] = { "brick", "stone", "metal" };
	for (int a = 0; a < 3; a++) {
		auto& m1 = ms[a] = Material::New();
		m1->shader(shd);
		m1->SetUniform("col", Color(1));
		m1->SetUniform("diffuseTex", Texture::New(IO::path() + "res/" + tp[a] + "_diffuse.jpg", opts));
		m1->SetUniform("normalTex", Texture::New(IO::path() + "res/" + tp[a] + "_normal.jpg", opts));
		m1->SetUniform("metalTex", Texture::New(IO::path() + "res/" + tp[a] + "_metal.jpg", opts));
		m1->SetUniform("roughTex", Texture::New(IO::path() + "res/" + tp[a] + "_rough.jpg", opts));
		m1->SetUniform("occluTex", Texture::New(IO::path() + "res/" + tp[a] + "_occlu.jpg", opts));
		m1->SetUniform("normalness", 1.f);
	}

	Mesh m = ProceduralMesh::UVSphere(32, 16, 1);
	Mesh m2 = MeshLoader::LoadObj(IO::path() + "res/monkey.obj");

	EImages::Init();
	EWindowManager::Init();
	ESerializer::Init();

	ss[0] = Background::New(IO::path() + "res/sky.hdr", 6);
	ss[1] = Background::New(IO::path() + "res/skyy.hdr", 6);
	ss[2] = Background::New(IO::path() + "res/skyyy.hdr", 6);

	ChokoLait::scene()->sky(ss[0]);
	ChokoLait::scene()->AddNewObject()
		->name("__Editor_Cameras__");
	auto o = ChokoLait::scene()->AddNewObject();
	o->name("Player");
	mr = o->AddComponent<MeshRenderer>();
	mr->mesh(m);
	//mr->materials({ m1, m2, m3 });
	mr->materials({ ms[0] });
	auto mr2 = ChokoLait::scene()->AddNewObject()->AddComponent<MeshRenderer>();
	mr2->object()->transform()->localPosition(Vec3(0.5f, 1, 0));
	mr2->mesh(m2);
	//mr->materials({ m1, m2, m3 });
	mr2->materials({ ms[0] });

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