#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"
#include "utils/meshutils.hpp"

CE_BEGIN_ED_NAMESPACE

Background ss;
Mesh mesh;
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
	UI::Texture(Rect(220, Display::height() - 220, 200, 200), tx);

	const Rect r3(Display::width() - 400, Display::height() - 420, 400, 400);

	glBlendFunc(GL_ONE, GL_ZERO);
	//UI::Texture(r3, dt.GetInfoTex(sz).jmpInfoTex->tex(0));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UI::Texture(r3, (Texture)tx2);

	#define tr(_v) r3.x() + r3.w() * _v.x, r3.y2() - r3.h() * _v.y - 1

	for (auto& t : mesh->triangles()) {
		Vec2 ts[3] = { mesh->texcoords()[t.x], mesh->texcoords()[t.y], mesh->texcoords()[t.z] };

		UI::Line(Vec2(tr(ts[0])), Vec2(tr(ts[1])), Color::white());
		UI::Line(Vec2(tr(ts[1])), Vec2(tr(ts[2])), Color::white());
		UI::Line(Vec2(tr(ts[2])), Vec2(tr(ts[0])), Color::white());
	}

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

		UI::Rect(Rect(tr(vj), 2, 2), Color::green());
	}

	MeshUtils::SurfaceBlur(dt, tx, tx2, tx2t, 10);
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

	tx = (Texture)EAssetList::Get(EAssetType::Texture, "t.png");
	sz = Int2(tx->width(), tx->height());

	TextureOptions opts = TextureOptions(TextureWrap::Clamp, TextureWrap::Clamp, 0, false);
	tx2t = RenderTarget::New(sz.x, sz.y, true, false, opts);
	tx2 = RenderTarget::New(sz.x, sz.y, true, false, opts);
	
	auto obj = (SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/a.blend/a.blend.prefab", true);
	Scene::AddObject(obj);
	auto mr = obj->children()[0]->GetComponent<MeshRenderer>();
	mesh = mr->mesh();
	dt = MeshUtils::GenSurfaceData(mesh);
	dt.GenInfoTex(sz);

	mr->materials({
		(Material)EAssetList::Get(EAssetType::Material, "unlit.material")
	});
	mr->materials()[0]->SetUniform("tex", (Texture)tx2);

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

	std::cout << Scene::Tree() << std::endl;

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