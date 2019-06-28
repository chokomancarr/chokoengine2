#include "chokoeditor.hpp"
#include "parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

MeshRenderer mr;
Material m1, m2;

Background ss[3];

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());

	ChokoLait::scene()->sky()->brightness(UI::I::Slider(Rect(10, 190, 100, 20), Vec2(0, 3), ChokoLait::scene()->sky()->brightness(), Color::yellow()));

	static bool um2 = false;
	if(Input::KeyDown(InputKey::M)) {
		mr->materials({ um2? m1 : m2 });
		um2 = !um2;
	}

	static int us2 = 1;
	if(Input::KeyDown(InputKey::B)) {
		ChokoLait::scene()->sky(ss[us2++]);
		if (us2 > 2)
			us2 = 0;
	}
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

	m1 = Material::New();
	m1->shader(shd);
	m1->SetUniform("col", Color(1));
	m1->SetUniform("diffuseTex", Texture::New(IO::path() + "res/brick_diffuse.jpg", opts));
	m1->SetUniform("normalTex", Texture::New(IO::path() + "res/brick_normal.jpg", opts));
	m1->SetUniform("metalTex", Texture::New(IO::path() + "res/brick_metal.jpg", opts));
	m1->SetUniform("roughTex", Texture::New(IO::path() + "res/brick_rough.jpg", opts));
	m1->SetUniform("occluTex", Texture::New(IO::path() + "res/brick_occlu.jpg", opts));
	m1->SetUniform("normalness", 1.f);

	m2 = Material::New();
	m2->shader(shd);
	m2->SetUniform("col", Color(1));
	m2->SetUniform("diffuseTex", Texture::New(IO::path() + "res/metal_diffuse.jpg", opts));
	m2->SetUniform("normalTex", Texture::New(IO::path() + "res/metal_normal.jpg", opts));
	m2->SetUniform("metalTex", Texture::New(IO::path() + "res/metal_metal.jpg", opts));
	m2->SetUniform("roughTex", Texture::New(IO::path() + "res/metal_rough.jpg", opts));
	m2->SetUniform("occluTex", Texture::New(IO::path() + "res/metal_occlu.jpg", opts));
	m2->SetUniform("normalness", 1.f);

	//Mesh m = MeshLoader::LoadObj(IO::path() + "res/monkey.obj");
	Mesh m = ProceduralMesh::UVSphere(32, 16, 1);

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
	mr->materials({ m1 });

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