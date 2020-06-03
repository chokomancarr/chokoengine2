#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"
#include "ce2/modules/ae/asset_loader.hpp"

CE_BEGIN_ED_NAMESPACE

Shader gshad;
std::vector<Vec2> gposs[3];
uint gi = 0;
VertexArray gvao[3];
Vec4 gcl[3] = {
	Vec4(1, 0, 0, 1),
	Vec4(0, 1, 0, 1),
	Vec4(1, 1, 0, 1),
};

void ig() {
	for (int i = 0; i < 3; i++)
		gposs[i].resize(100);
	for (int a = 0; a < 100; a++) {
		for (int i = 0; i < 3; i++)
			gposs[i][a] = Vec2(-0.95f + 0.005f * a, -0.95f);
	}

	(gshad = Shader::New(R"(
layout(location=0) in vec2 pos;
void main() {
    gl_Position = vec4(pos, 0, 1);
})", R"(
uniform vec4 col;
out vec4 fragCol;
void main() {
    fragCol = col;
})"
	))->AddUniforms({"col"});
	
	for (int i = 0; i < 3; i++) {
		(gvao[i] = VertexArray_New())
			->AddBuffer(VertexBuffer_New(true, 2, 100, gposs[i].data()));
	}
}

void pg(uint64_t* f) {
	if (gi == 100) {
		for (int a = 1; a < 100; a++) {
			for (int i = 0; i < 3; i++)
				gposs[i][a - 1].y = gposs[i][a].y;
		}
	}
	else gi++;

	for (int i = 0; i < 3; i++) {
		gposs[i][gi - 1].y = -0.95f + 0.01f * f[i];
		gvao[i]->buffer(0)->Set(gposs[i].data(), 100);
	}

	gshad->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 2; i >= 0; i--) {
		glUniform4f(gshad->Loc(0), gcl[i].r, gcl[i].g, gcl[i].b, 1);
		gvao[i]->Bind();
		glDrawArrays(GL_LINE_STRIP, 0, gi);
		gvao[i]->Unbind();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gshad->Unbind();

	const std::string nms[] = {
		"sync",
		"update",
		"paint"
	};
	for (int a = 0; a < 3; a++) {
		UI::Rect(Rect(10 + 70 * a, Display::height() - 40, 16, 16), gcl[a]);
		UI::Label(Rect(30 + 70 * a, Display::height() - 40, 16, 16), nms[a], Color::white());
	}
}

uint64_t gt0, gt[3], dt[3];

inline void paint() {
	gt[1] = Time::actualMillis();
	dt[1] = gt[1] - gt0;

	ChokoEditor::scene->PrepareSceneForRendering();
	EWindowManager::Render();

	UI::Texture(Display::fullscreenRect(), EImages::background, UIScaling::Crop, Color(0.5f));
	EWindowManager::Draw();

	EOverlayManager::Draw();

	if (!EDragDrop::IsEmpty()) {
		Vec2 pos = Input::mousePosition() + Vec2(10, 5);
		for (auto& t : EDragDrop::target) {
			UI::Label(Rect(pos.x, pos.y, 1000, 20), t, Color(1, 0.8f));
			pos.y += 14;
		}
		for (auto& t : EDragDrop::targetObj) {
			UI::Label(Rect(pos.x, pos.y, 1000, 20), t->name(), Color(1, 0.8f));
			pos.y += 14;
		}
	}

	gt[2] = Time::actualMillis();
	dt[2] = (gt[2] - gt0);
	gt0 = gt[2];
	UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(dt[2]) + " ms", Color::white());

	pg(dt);
}

std::string ChokoEditor::projectRoot;

Scene ChokoEditor::scene;

void ChokoEditor::Init() {
	_DummyScriptLoader::Init();
}

void ChokoEditor::Main() {
	ChokoLait::InitOptionsSt opts;
	opts.title = "ChokoEditor";
	ChokoLait::Init(1000, 600, opts);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	projectRoot = IO::ReadFile(IO::userPath() + ".ce/lastopenproject.txt");
	ModuleAE::AssetLoader::Init(CE_DIR_ASSET);

	EPaths::Init();
	EAssetList::Init();
	EImages::Init();
	EIcons::Init();
	EWindowManager::Init();
	EOverlayManager::Init();
	EScripting::Init();
	EPlayer::Init();

	EAssetList::Rescan();

	scene = Scene::New();
	scene->callbackFlags(CompCallbackFlag::EXEC_IN_EDIT_MODE);

	EAssetManager::Init();
	ESceneManager::Init();

	EProjectBuilder::Init();

	ESceneManager::LoadLastOpened();

	std::cout << EDebug::ObjTree(scene->objects()[1]->children(), [](const SceneObject& o) {
		std::string res = o->name() + " " + std::to_string(o->id()) + ": ";
		auto info = o->prefabInfo();
		if (!info.prefab) {
			res += "-";
		}
		else {
			res += info.prefab->assetSignature() + " i" + std::to_string(info.id) + " ";
			if (!!info.head) {
				res += "h" + std::to_string(info.head->id()) + " ";
			}
			res += "[";
			for (auto& i : info.ids) {
				res += std::to_string(i) + ", ";
			}
			res += "]";
		}
		return res;
	}) << std::endl;

	if (ESceneManager::activeScenePath.empty()) {
		scene->sky((Background)EAssetList::Get(AssetType::Background, "sky2.hdr"));
		auto rb = ((Prefab)EAssetList::Get(AssetType::Prefab,
			".exported/monkey.blend/monkey.blend.prefab")
			)->Instantiate([](AssetType t, const std::string& s) -> Asset {
			return EAssetList::Get(t, s, true);
		});
		scene->AddObject(rb, scene->objects()[1]);
		rb->AddComponent<DummyScript>();

		const auto& oc = scene->AddNewObject(scene->objects()[1]);
		oc->transform()->localPosition(Vec3(0, 0, 2));
		oc->transform()->localRotationEuler(Vec3(0, 180, 0));
		oc->AddComponent<Camera>();

		ESceneManager::activeScenePath = "aaa.scene";
	}

	Debug::Message("Editor", "Loading windows");
	EWindowManager::LoadWindows();

	UIButtonStyle style(Color(0.1f, 1));
	style.textNormal(Color::white());

	Debug::Message("Editor", "Startup finished");

    //EAssetArchiver::Exec(projectRoot + "build/assets");

	ig();

	while (ChokoLait::alive()) {
		dt[0] = (gt[0] = Time::actualMillis()) - gt0;
		ChokoLait::Update([]() {
			UI_Ext::PreLoop();
			EWindowManager::Update();
			EOverlayManager::Update();

			scene->Update();
			scene->LateUpdate();
			scene->PostLogic();
		});
		ChokoLait::Paint(0, paint);
		EDragDrop::PostLoop();
	}
}

CE_END_ED_NAMESPACE
