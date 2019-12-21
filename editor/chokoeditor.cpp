#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"
#include "utils/meshutils.hpp"

CE_BEGIN_ED_NAMESPACE

Background ss;
Mesh mesh;
MeshSurfaceData dt;
Int2 sz;
Texture tx, txp;
RenderTarget tx2t;
RenderTarget tx2;
Material mat;

inline void paint() {
	UI::Texture(Display::fullscreenRect(), EImages::background, UIScaling::Crop, Color(0.5f));
	EWindowManager::Draw();

	EOverlayManager::Draw();

	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(Time::delta() * 1000) + " ms", Color::white());

	//UI::Texture(Rect(10, Display::height() - 300, 300, 300), tx);
	//UI::Texture(Rect(320, Display::height() - 300, 300, 300), txp);

	const Rect r3(Display::width() - 800, Display::height() - 800, 800, 800);

	glBlendFunc(GL_ONE, GL_ZERO);
	//UI::Texture(r3, dt.GetInfoTex(sz).jmpInfoTex->tex(0));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UI::Texture(r3, (Texture)tx2);

	#define tr(_v) r3.x() + r3.w() * _v.x, r3.y2() - r3.h() * _v.y - 1

	static bool wob = false;

	wob = UI::I::Toggle(Rect(r3.x(), r3.y() - 30, 100, 20), wob, Color(0.3f));

	const Color lc = wob ? Color::black() : Color::white();

	if (wob) UI::Rect(r3, Color::white());

	for (auto& t : mesh->triangles()) {
		Vec2 ts[3] = { mesh->texcoords()[t.x], mesh->texcoords()[t.y], mesh->texcoords()[t.z] };

		UI::Line(Vec2(tr(ts[0])), Vec2(tr(ts[1])), lc);
		UI::Line(Vec2(tr(ts[1])), Vec2(tr(ts[2])), lc);
		UI::Line(Vec2(tr(ts[2])), Vec2(tr(ts[0])), lc);
	}

	if (wob) return;

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
		Int4 vj;
		glReadPixels(px, py, 1, 1, GL_RGBA_INTEGER, GL_INT, &vj);
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

		Vec2 pj = Vec2(vj.x - 1, vj.y - 1) / 100000.f;
		UI::Rect(Rect(tr(pj), 2, 2), Color::green());
		UI::Rect(Rect(r3.x() + r3.w() * vc[0], r3.y2() - r3.h() * vc[1] - 1, 4, 4), Color::red());
	}

	static int nb = 1;

	nb = StrExt::ToInt(UI::I::TextField(Rect(20, 20, 100, 20), std::to_string(nb), Color(0.2f)), 1);

	static float scl = 1;

	scl = UI::I::Slider(Rect(20, 50, 100, 20), Vec2(0.8f, 1.2f), scl, Color(0.3f));

	static float mscl = 1;

	mscl = UI::I::Slider(Rect(20, 80, 100, 20), Vec2(0.5f, 3.f), mscl, Color(0.3f));

	static bool show = true;

	if (UI::I::Toggle(Rect(20, 110, 20, 20), show, Color(0.2f)) != show) {
		show = !show;
		mat->SetUniform("tex", show ? (Texture)tx2 : tx);
	}

	Scene::objects()[1]->transform()->localScale(mscl);

	//MeshUtils::SurfaceBlur(dt, txp, tx2, tx2t, scl);
	for (int a = 0; a < nb; a++) {
		MeshUtils::SurfaceBlur(dt, !a ? txp : (Texture)tx2, tx2, tx2t, scl);
	}
}

std::string ChokoEditor::assetPath;

void ChokoEditor::Init() {
	
}

//#define DRAGON 1
//#define kcschan 1
//#define useobj 1

#ifdef DRAGON
#define tex "t2"
#define model "dragon full"
#elif kcschan
#define tex "skin"
#define model "kcschan"
#elif 1
#define tex "grid2"
#define model "ball4"
#else 
#define tex "t"
#define model "a"
#endif

void ChokoEditor::Main() {
	ChokoLait::Init("ChokoEditor", 1600, 1000);

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

	tx = (Texture)EAssetList::Get(EAssetType::Texture, tex ".png");
	sz = Int2(tx->width(), tx->height());

	TextureOptions opts = TextureOptions(TextureWrap::Clamp, TextureWrap::Clamp, 0, false);
	txp = (Texture)RenderTarget::New(sz.x, sz.y, true, false, opts);
	tx2t = RenderTarget::New(sz.x, sz.y, true, false, opts);
	tx2 = RenderTarget::New(sz.x, sz.y, true, false, opts);
	
#ifdef useobj
	auto obj = Scene::AddNewObject();
	Scene::AddNewObject(obj) ->AddComponent<MeshRenderer>()->mesh(
		(Mesh)EAssetList::Get(EAssetType::Mesh, model ".obj")
	);
#else
	auto obj = (SceneObject)EAssetList::Get(EAssetType::SceneObject, ".exported/" model ".blend/" model ".blend.prefab", true);
	Scene::AddObject(obj);
#endif
	
	auto mr = obj->children()[0]->GetComponent<MeshRenderer>();
	mesh = mr->mesh();
	dt = MeshUtils::GenSurfaceData(mesh);
	dt.GenInfoTex(sz);
	MeshUtils::PadTexture(dt, tx, (RenderTarget)txp);

	mr->materials({
		(Material)EAssetList::Get(EAssetType::Material, "unlit.material")
	});
	mat = mr->materials()[0];
	mat->SetUniform("tex", (Texture)tx2);

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

	std::cout << Scene::Tree() << std::endl;

	Debug::Message("Editor", "Startup finished");

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			UI_Ext::PreLoop();
			EWindowManager::Update();
			EOverlayManager::Update();

			//Scene::objects()[1]->transform()->localRotationEuler(Vec3(0, Time::time() * 45, 0));
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_ED_NAMESPACE