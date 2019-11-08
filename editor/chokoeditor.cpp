#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"
#include "utils/meshutils.hpp"

CE_BEGIN_ED_NAMESPACE

Background ss;
MeshSurfaceData dt;
Int2 sz;
Texture tx;
Texture txp;
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
	UI::Texture(Rect(430, Display::height() - 90, 70, 70), tx);
	UI::Texture(Rect(500, Display::height() - 90, 70, 70), txp);

	const Rect r3(Display::width() - 300, Display::height() - 320, 300, 300);

	glBlendFunc(GL_ONE, GL_ZERO);
	//UI::Texture(r3, dt.GetInfoTex(sz).jmpInfoTex->tex(0));
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

	MeshUtils::SurfaceBlur(dt, (Texture)txp, tx2, tx2t, 10);
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
	txp = (Texture)RenderTarget::New(sz.x, sz.y, true, false, opts);
	tx2t = RenderTarget::New(sz.x, sz.y, true, false, opts);
	tx2 = RenderTarget::New(sz.x, sz.y, true, false, opts);
	
	auto obj = (SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/a.blend/a.blend.prefab", true);
	Scene::AddObject(obj);
	auto mr = obj->children()[0]->GetComponent<MeshRenderer>();
	dt = MeshUtils::GenSurfaceData(mr->mesh());
	dt.GenInfoTex(sz);
	MeshUtils::PadTexture(dt, tx, (RenderTarget)txp);
	txp = tx;

	mr->materials({
		(Material)EAssetList::Get(EAssetType::Material, "unlit.material")
	});
	mr->materials()[0]->SetUniform("tex", (Texture)tx2);

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

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