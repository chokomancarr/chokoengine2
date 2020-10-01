#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESceneManager::Ops {

public:
    static void Reg();

    CE_DEF_CALLBACK(AddNewObject);
    CE_DEF_CALLBACK(AddNewObjectAsChild);

    CE_DEF_CALLBACK(DeleteObject);

	CE_DEF_CALLBACK(RemoveTopPrefabLink);
	CE_DEF_CALLBACK(RemovePrefabLinks);
};

CE_END_ED_NAMESPACE