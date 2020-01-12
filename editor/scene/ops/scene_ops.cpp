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

CE_END_ED_NAMESPACE