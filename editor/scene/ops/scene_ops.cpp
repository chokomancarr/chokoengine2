#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	CE_IMPL_CALLBACK(AddNewObject) {
		ChokoEditor::scene->AddNewObject(ESceneManager::sceneBaseObj());
	}
	CE_IMPL_CALLBACK(AddNewObjectAsChild) {
		ChokoEditor::scene->AddNewObject(args.Get<SceneObject>("object"));
	}

	CE_IMPL_CALLBACK(DeleteObject) {
		ChokoEditor::scene->RemoveObject(args.Get<SceneObject>("object"));
	}

	CE_IMPL_CALLBACK(RemoveTopPrefabLink) {
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

	CE_IMPL_CALLBACK(RemovePrefabLinks) {

	}

	CE_IMPL_CALLBACK(ApplyPrefabOne) {
		auto obj = args.Get<SceneObject>("object");
		auto& info = obj->prefabInfo();
		const auto it = std::find_if(info.uids.rbegin(), info.uids.rend(),
				[](const _SceneObject::PrefabInfo::UID& uid) {
			return !uid.prefab->readonly;
		});
		if (it != info.uids.rend()) {
			auto prb = it->prefab.lock();
			prb->Apply(obj);
			EAssetWriter::Write(prb);
		}
		else {
			EDebug::Log("SceneOps", "object does not have non-readonly prefab info!");
		}
	}

	CE_IMPL_CALLBACK(ApplyPrefabTree) {
		auto obj = args.Get<SceneObject>("object");
		auto& info = obj->prefabInfo();
		const auto it = std::find_if(info.uids.rbegin(), info.uids.rend(),
			[](const _SceneObject::PrefabInfo::UID& uid) {
			return !uid.prefab->readonly;
		});
		if (it != info.uids.rend()) {
			if (!!it->id) {
				EDebug::Log("SceneOps", "prefab tree can only be applied from root object!");
			}
			auto prb = it->prefab.lock();
			prb->Apply(obj, true);
			EAssetWriter::Write(prb);
		}
		else {
			EDebug::Log("SceneOps", "object does not have non-readonly prefab info!");
		}
	}

	CE_IMPL_CALLBACK(RevertPrefabOne) {
		auto obj = args.Get<SceneObject>("object");
		auto& info = obj->prefabInfo();

		info.uids[0].prefab->Revert(obj);
	}
}

void ESceneManager::Ops::Reg() {
	ECallbackManager::Register(CallbackSig::OBJECT_NEW, AddNewObject);
	ECallbackManager::Register(CallbackSig::OBJECT_NEW_CHILD, AddNewObjectAsChild);
	ECallbackManager::Register(CallbackSig::OBJECT_DELETE, DeleteObject);
	ECallbackManager::Register(CallbackSig::OBJECT_PREFAB_APPLY_SINGLE, ApplyPrefabOne);
	ECallbackManager::Register(CallbackSig::OBJECT_PREFAB_APPLY_TREE, ApplyPrefabTree);
	ECallbackManager::Register(CallbackSig::OBJECT_PREFAB_RESET, RevertPrefabOne);
}

CE_END_ED_NAMESPACE