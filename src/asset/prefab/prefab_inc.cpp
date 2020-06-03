#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

namespace PrefabState {
    _Prefab::_Sig2Prb sig2PrbFn = nullptr;
    _Prefab::_Sig2Ass sig2AssFn = nullptr;

	std::vector<pPrefab> prefabStack = {};

	std::stack<SceneObject> activeBaseObjs = {};

	std::stack<std::vector<std::function<void()>>> refresolvers = {};
}

CE_END_NAMESPACE