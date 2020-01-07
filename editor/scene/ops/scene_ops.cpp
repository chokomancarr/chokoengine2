#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void ESceneManager::Ops::Reg() {
    ECallbackManager::Register(CallbackSig::OBJECT_NEW, AddNewObject);
    ECallbackManager::Register(CallbackSig::OBJECT_NEW_CHILD, AddNewObjectAsChild);
    ECallbackManager::Register(CallbackSig::OBJECT_DELETE, DeleteObject);
}

CE_IMPL_CALLBACK(ESceneManager::Ops::AddNewObject) {
	ChokoEditor::scene->AddNewObject(ChokoEditor::scene->objects()[1]);
}
CE_IMPL_CALLBACK(ESceneManager::Ops::AddNewObjectAsChild) {
	ChokoEditor::scene->AddNewObject(ESceneInfo::selectedObject);
}

CE_IMPL_CALLBACK(ESceneManager::Ops::DeleteObject) {
	ChokoEditor::scene->RemoveObject(ESceneInfo::selectedObject);
}

CE_END_ED_NAMESPACE