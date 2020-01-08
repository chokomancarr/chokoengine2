#include "ce2/modules/pe/player_debug.hpp"
#include "loader_base.hpp"

namespace CEPE = CE_NS ModulePE;

int main() {
	CE_PL_NS _ScriptLoaderImpl::Init();
	CEPE::PlayerDebug::Init();

	CE_PL_NS SceneManager::scenePaths.push_back(CEPE::PlayerDebug::projectPath + "assets/.ce/startup.scene");

	CE_NS ChokoLait::scene = CE_PL_NS SceneManager::Load(0);

	CEPE::PlayerDebug::Main();
}