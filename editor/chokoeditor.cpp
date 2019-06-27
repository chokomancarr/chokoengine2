#include "chokoeditor.hpp"
#include "parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

Material m1, m2, m3;

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());

	ChokoLait::scene()->sky()->brightness(UI::I::Slider(Rect(10, 190, 100, 20), Vec2(0, 3), ChokoLait::scene()->sky()->brightness(), Color::yellow()));
}

void ChokoEditor::Init() {
	
}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 1000, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	Shader shd = Shader::New(IO::ReadFile(IO::path() + "res/color copy.vs"), IO::ReadFile(IO::path() + "res/color copy.fs"));
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

	m1 = Material::New();
	m1->shader(shd);
	m1->SetUniform("col", Color(1));
	m1->SetUniform("diffuseTex", Texture::New(IO::path() + "res/metal_diffuse.jpg"));
	m1->SetUniform("normalTex", Texture::New(IO::path() + "res/metal_normal.jpg"));
	m1->SetUniform("metalTex", Texture::New(IO::path() + "res/metal_metal.jpg"));
	m1->SetUniform("roughTex", Texture::New(IO::path() + "res/metal_rough.jpg"));
	m1->SetUniform("occluTex", Texture::New(IO::path() + "res/metal_occlu.jpg"));
	m1->SetUniform("normalness", 1.f);

	//Mesh m = MeshLoader::LoadObj(IO::path() + "res/monkey.obj");
	Mesh m = ProceduralMesh::UVSphere(32, 16, 1);

	EImages::Init();
	EWindowManager::Init();
	ESerializer::Init();

	ChokoLait::scene()->sky(Background::New(IO::path() + "res/sky.hdr", 6));
	ChokoLait::scene()->AddNewObject()
		->name("__Editor_Cameras__");
	auto o = ChokoLait::scene()->AddNewObject();
	o->name("Player");
	auto mr = o->AddComponent<MeshRenderer>();
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