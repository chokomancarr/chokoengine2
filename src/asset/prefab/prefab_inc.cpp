#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

namespace PrefabState {
    _Prefab::_Sig2Prb sig2PrbFn = nullptr;
    _Prefab::_Sig2Ass sig2AssFn = nullptr;

	std::stack<Prefab> activePrefabs = {};

	std::stack<SceneObject> activeBaseObjs = {};
}

CE_END_NAMESPACE