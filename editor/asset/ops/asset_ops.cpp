#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {

	CE_IMPL_CALLBACK(Refresh) {

	}
	CE_IMPL_CALLBACK(RefreshAll) {

	}

	CE_IMPL_CALLBACK(Reimport) {
		const auto& sig = args["sig"].s;
		EDebug::Log("AssetManager", "Reimporting " + sig);
		EAssetList::Reimport(sig);
	}
	CE_IMPL_CALLBACK(ReimportAll) {

	}

	CE_IMPL_CALLBACK(Delete) {

	}

	CE_IMPL_CALLBACK(PrefabCreate) {
		const auto& obj = args.Get<SceneObject>("obj");
		std::ofstream(CE_DIR_ASSET + args["path"].s + obj->name() + ".prefab")
			<< JsonParser::Export(Prefab::New(obj, true)->ToJson());
	}
	CE_IMPL_CALLBACK(PrefabSpawn) {
		ChokoEditor::scene->AddObject(
			((Prefab)args[0].o.lock())->Instantiate(EAssetList::Get2),
			ESceneManager::sceneBaseObj()
		);
	}
	CE_IMPL_CALLBACK(PrefabApply) {

	}

}

void EAssetManager::Ops::Reg() {
	ECallbackManager::Register(CallbackSig::ASSET_REFRESH, Refresh);
	ECallbackManager::Register(CallbackSig::ASSET_REIMPORT, Reimport);
	ECallbackManager::Register(CallbackSig::ASSET_DELETE, Delete);

	ECallbackManager::Register(CallbackSig::ASSET_PREFAB_CREATE, PrefabCreate);
	ECallbackManager::Register(CallbackSig::ASSET_PREFAB_SPAWN, PrefabSpawn);
}

CE_END_ED_NAMESPACE