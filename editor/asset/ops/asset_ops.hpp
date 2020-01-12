#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EAssetManager::Ops {

public:
    static void Reg();

    CE_DEF_CALLBACK(Refresh);
	CE_DEF_CALLBACK(RefreshAll);
	CE_DEF_CALLBACK(Reimport);
	CE_DEF_CALLBACK(ReimportAll);
	CE_DEF_CALLBACK(Delete);

    CE_DEF_CALLBACK(PrefabSpawn);
	CE_DEF_CALLBACK(PrefabApply);
};

CE_END_ED_NAMESPACE