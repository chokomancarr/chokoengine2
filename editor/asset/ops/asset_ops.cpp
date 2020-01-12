#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EAssetManager::Ops::Reg() {
    ECallbackManager::Register(CallbackSig::ASSET_REFRESH, Refresh);
    ECallbackManager::Register(CallbackSig::ASSET_REIMPORT, Reimport);
	ECallbackManager::Register(CallbackSig::ASSET_DELETE, Delete);

	ECallbackManager::Register(CallbackSig::PREFAB_SPAWN, PrefabSpawn);
	ECallbackManager::Register(CallbackSig::PREFAB_APPLY, PrefabApply);
}

CE_IMPL_CALLBACK(EAssetManager::Ops::Refresh) {

}
CE_IMPL_CALLBACK(EAssetManager::Ops::RefreshAll) {

}

CE_IMPL_CALLBACK(EAssetManager::Ops::Reimport) {

}
CE_IMPL_CALLBACK(EAssetManager::Ops::ReimportAll) {

}

CE_IMPL_CALLBACK(EAssetManager::Ops::Delete) {

}

CE_IMPL_CALLBACK(EAssetManager::Ops::PrefabSpawn) {
	ChokoEditor::scene->AddObject(
		((Prefab)args[0].o.lock())->Instantiate(EAssetList::Get2),
		ESceneManager::sceneBaseObj()
	);
}

CE_IMPL_CALLBACK(EAssetManager::Ops::PrefabApply) {

}

CE_END_ED_NAMESPACE