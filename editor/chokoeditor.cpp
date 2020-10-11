#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "ext/ui_ext.hpp"
#include "ce2/modules/ae/asset_loader.hpp"
#include "prefs/preferences.hpp"

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

bool runphys = false;

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
		glUniform4f(gshad->Loc(0), gcl[i].x, gcl[i].y, gcl[i].z, 1);
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
	else {
		if (EPreferences::showSettings) {
			UI_Ext::IncLayer();
			EPreferences::Draw();
		}
		else {
			if (Input::KeyDown(InputKey::U) && Input::KeyHold(InputKey::LeftControl) && Input::KeyHold(InputKey::LeftAlt)) {
				EPreferences::showSettings = true;
			}
		}
	}


	static bool showdebug = false;
	if (Input::KeyDown(InputKey::D) && Input::KeyHold(InputKey::LeftControl) && Input::KeyHold(InputKey::LeftAlt)) {
		showdebug = !showdebug;
	}

	if (showdebug) {
		gt[2] = Time::actualMillis();
		dt[2] = (gt[2] - gt0);
		gt0 = gt[2];
		UI::Label(Rect(10, Display::height() - 20, 100, 20), std::to_string(dt[2]) + " ms", Color::white());

		pg(dt);
	}

	/*
	const Vec2 r0(Display::width() - 250, Display::height() - 200);

	UI::Rect(Rect(r0.x, r0.y, 240, 190), Color(0.2f, 0.7f));
	UI::Label(Rect(r0.x + 5, r0.y + 5, 200, 20), "Physics", Color(0.8f));
	UI::Label(Rect(r0.x + 10, r0.y + 30, 200, 20), "simulating", Color(0.8f));
	runphys = UI::I::Toggle(Rect(r0.x + 200, r0.y + 30, 16, 16), runphys, Color(0.3f));
	Physics::timeScale(runphys ? 1 : 0);
	*/
}

std::string ChokoEditor::projectRoot;

Scene ChokoEditor::scene;

void ChokoEditor::Init() {
	_DummyScriptLoader::Init();
}

void ChokoEditor::Main() {
	ChokoLait::InitOptionsSt opts;
	opts.title = "ChokoEditor";
	//opts.enablePhysics = true;
	ChokoLait::Init(1000, 600, opts);

	//Physics::timeScale(0);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	projectRoot = IO::ReadFile(IO::userPath() + ".ce/lastopenproject.txt");
	ModuleAE::AssetLoader::Init(CE_DIR_ASSET);

	EPaths::Init();
	EPreferences::Init();
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
		if (!info.uids.size()) {
			res += "-";
		}
		else {
			for (auto& uid : info.uids) {
				res += "[" + uid.prefab->assetSignature() + " " + std::to_string(uid.id) + "] ";
			}
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
			Physics::Update(scene);
			scene->LateUpdate();
			scene->PostLogic();
		});
		ChokoLait::Paint(0, paint);
		EDragDrop::PostLoop();
	}
}

CE_END_ED_NAMESPACE
