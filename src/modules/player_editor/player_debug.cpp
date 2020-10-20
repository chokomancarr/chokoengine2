#include "player_debug.hpp"
#include "ce2/modules/ae/asset_loader.hpp"

CE_BEGIN_MOD_PE_NAMESPACE

std::string PlayerDebug::projectPath = "";

Input::State PlayerDebug::inputState = {};

void PlayerDebug::Init() {
	PDSyncer::Init();

	const auto& mem = PDSyncer::GetBaseSt();

	Debug::Message("ChokoPlayerD", "Init with " + std::to_string(mem.screen_width) + " x " + std::to_string(mem.screen_height));

	ChokoLait::InitOptionsSt opts = {};
	ChokoLait::Init(mem.screen_width, mem.screen_height, opts);

	projectPath = IO::path();
	for (int a = 0; a < 3; a++) {
		projectPath = StrExt::ParentFd(projectPath);
	}
	Debug::Message("ChokoPlayerD", "Project root is: " + projectPath);

	auto font = Font::New(projectPath + "assets/.internal/defaultFont.ttf");
	UI::defaultFont(font);
	font->size(12);

    Player::ChokoPlayer::Init();
	ModuleAE::AssetLoader::Init(CE_MOD_PE_PATH_ASSET);
	Debug::Message("ChokoPlayerD", "Init finish");
}

inline void paint() {

}

void PlayerDebug::Main() {
	Player::SceneManager::scenes.push_back(
		Player::SceneManager::Entry{
			"__default",
			projectPath + "assets/.ce/startup.scene"
		}
	);

	Debug::Message("ChokoPlayerD", "Loading startup scene");

	ChokoLait::scene = Player::SceneManager::Load(0);

	Debug::Message("ChokoPlayerD", "Loading done");

	std::cout << ChokoLait::scene->Tree() << std::endl;

	Time::Reset();

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			Input::state(inputState);
		});
		ChokoLait::Paint(0, paint);

		if (!PDSyncer::SyncFrame())
			break;
	}

	PDSyncer::Cleanup();
}

CE_END_MOD_PE_NAMESPACE
