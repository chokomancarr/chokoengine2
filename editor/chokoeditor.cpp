#include "chokoeditor.hpp"
#include "parsers/mesh.hpp"

CE_BEGIN_ED_NAMESPACE

Material m1, m2, m3;

void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, Color::gray(0.5f));
	//UI::Texture(Rect(Display::width() * 0.5f - 64, Display::height() * 0.5f - 64, 128, 128), EImages::logo);
	EWindowManager::Draw();
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());
}

void ChokoEditor::Init() {
	
}

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 1000, 600);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	Shader shd = Shader::New(IO::ReadFile(IO::path() + "res/color.vs"), IO::ReadFile(IO::path() + "res/color.fs"));
	shd->AddUniform("_MV", ShaderVariableType::Matrix);
	shd->AddUniform("_P", ShaderVariableType::Matrix);
	shd->AddUniform("_MVP", ShaderVariableType::Matrix);
	shd->AddUniform("col", ShaderVariableType::Vec4);

	const float f = 0.3;

	m1 = Material::New();
	m1->shader(shd);
	m1->SetUniform("col", Color(1, f, f));
	m2 = Material::New();
	m2->shader(shd);
	m2->SetUniform("col", Color(f, 1, f));
	m3 = Material::New();
	m3->shader(shd);
	m3->SetUniform("col", Color(f, f, 1));

	Mesh m = MeshLoader::LoadObj(IO::path() + "res/monkey.obj");

	EImages::Init();
	EWindowManager::Init();
	ESerializer::Init();

	ChokoLait::scene()->sky(static_cast<Texture>(Background::New(IO::path() + "res/sky.hdr", 6)));
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