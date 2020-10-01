#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void ESceneManager::Ops::Reg() {
    ECallbackManager::Register(CallbackSig::OBJECT_NEW, AddNewObject);
    ECallbackManager::Register(CallbackSig::OBJECT_NEW_CHILD, AddNewObjectAsChild);
    ECallbackManager::Register(CallbackSig::OBJECT_DELETE, DeleteObject);
}

CE_IMPL_CALLBACK(ESceneManager::Ops::AddNewObject) {
	ChokoEditor::scene->AddNewObject(ESceneManager::sceneBaseObj());
}
CE_IMPL_CALLBACK(ESceneManager::Ops::AddNewObjectAsChild) {
	ChokoEditor::scene->AddNewObject(args.Get<SceneObject>("object"));
}

CE_IMPL_CALLBACK(ESceneManager::Ops::DeleteObject) {
	ChokoEditor::scene->RemoveObject(args.Get<SceneObject>("object"));
}

CE_IMPL_CALLBACK(ESceneManager::Ops::RemoveTopPrefabLink) {
	auto obj = args.Get<SceneObject>("object");
	const auto& info = obj->prefabInfo();
	if (!!info.uids.size()) {
		const auto it = std::find_if(info.uids.begin(), info.uids.end(),
				[](const _SceneObject::PrefabInfo::UID& uid) {
			return !uid.id;
		});
		if (it != info.uids.end()) {
			const int i = it - info.uids.begin();
			for (auto& po : it->objs) {
				auto o = po.lock();
				auto info2 = o->prefabInfo();
				info2.uids.erase(info2.uids.begin() + i);
				o->prefabInfo(info2);
			}
		}
		else {
			EDebug::Log("SceneManager::Ops", "object is not head of spawned prefab!");
		}
	}
	else {
		EDebug::Log("SceneManager::Ops", "object does not have prefab info!");
	}
}

CE_IMPL_CALLBACK(ESceneManager::Ops::RemovePrefabLinks) {

}

CE_END_ED_NAMESPACE