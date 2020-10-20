#include "player_release.hpp"
#include "ce2/modules/aa/asset_loader.hpp"

CE_BEGIN_MOD_PR_NAMESPACE

void Init() {
	Debug::Message("ChokoPlayerR", "Initializing");
	ChokoLait::InitOptionsSt opts = {};
	ChokoLait::Init(1366, 768, opts);

	auto font = Font::New(IO::path() + "defaultFont.ttf");
	UI::defaultFont(font);
	font->size(12);

    Player::ChokoPlayer::Init();
	Player::SceneManager::Init();
	ModuleAA::AssetLoader::Init(IO::path());
	Debug::Message("ChokoPlayerR", "Init finish");
}

void RegScenes() {
	std::ifstream strm(IO::path() + "scene_index");
	if (!strm) {
		Debug::Error("ChokoPlayerR", "fatal: cannot open scene_index file!");
		CE_ABORT();
	}
	std::string s;
	int i = 0;
	while (std::getline(strm, s)) {
		if (s.empty()) continue;
		Player::SceneManager::scenes.push_back(
			Player::SceneManager::Entry{
				s, IO::path() + "scene" + std::to_string(i++) + ".dat"
			}
		);
	}
}

void PlayerRelease::Main() {
	Init();
	RegScenes();

	Debug::Message("ChokoPlayerR", "Loading startup scene");

	ChokoLait::scene = Player::SceneManager::Load(0);

	Debug::Message("ChokoPlayerR", "Loading done");

	std::cout << ChokoLait::scene->Tree() << std::endl;

	Time::Reset();

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
		});
		ChokoLait::Paint(0, []() {

		});
	}
}

CE_END_MOD_PR_NAMESPACE
