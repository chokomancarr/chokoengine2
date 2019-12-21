#include "player_debug.hpp"
#include "ce2/modules/ae/asset_loader.hpp"

CE_BEGIN_MOD_PE_NAMESPACE

std::string PlayerDebug::projectPath = "";

void PlayerDebug::Init() {
	PDSyncer::Init();

	const auto& mem = PDSyncer::GetBaseSt();

	ChokoLait::InitOptionsSt opts = {};
	ChokoLait::Init(mem.screen_width, mem.screen_height, opts);

	projectPath = IO::path();
	for (int a = 0; a < 4; a++) {
		projectPath = projectPath.substr(0,
			projectPath.find_last_of('/'));
	}
	projectPath += '/';
	Debug::Message("ChokoPlayerD", "Project root is: " + projectPath);

    Player::ChokoPlayer::Init();
	ModuleAE::AssetLoader::Init(CE_MOD_PE_PATH_ASSET);
}

inline void paint() {

}

void PlayerDebug::Main() {
	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {

		});
		ChokoLait::Paint(0, paint);

		PDSyncer::SyncFrame();
	}
}

CE_END_MOD_PE_NAMESPACE
