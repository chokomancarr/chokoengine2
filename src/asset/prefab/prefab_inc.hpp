#pragma once
#include "chokoengine.hpp"

#define CE_GET_IT(it, nm, vl) auto it = std::find(nm.begin(), nm.end(), vl);\
	if(it != nm.end())

CE_BEGIN_NAMESPACE

namespace PrefabState {
    extern _Prefab::_Sig2Prb sig2PrbFn;
    extern _Prefab::_Sig2Ass sig2AssFn;

	extern std::vector<pPrefab> prefabStack;

	extern std::stack<SceneObject> activeBaseObjs;

	extern std::stack<std::unordered_set<size_t>> ids_indirect;

	extern std::stack<std::vector<std::function<void()>>> refresolvers;
}

typedef _Prefab::_ObjBase _PrefabObjBase;
typedef std::unique_ptr<_PrefabObjBase> PrefabObjBase;

class _PrefabObj;
typedef std::unique_ptr<_PrefabObj> PrefabObj;
#define PrefabObj_New(...) PrefabObj(new _PrefabObj(__VA_ARGS__))
class _PrefabLink;
typedef std::unique_ptr<_PrefabLink> PrefabLink;
#define PrefabLink_New(...) PrefabLink(new _PrefabLink(__VA_ARGS__))
class _PrefabMod;
typedef std::unique_ptr<_PrefabMod> PrefabMod;
#define PrefabMod_New(...) PrefabMod(new _PrefabMod(__VA_ARGS__))
class _PrefabComp;
typedef std::unique_ptr<_PrefabComp> PrefabComp;
#define PrefabComp_New(...) PrefabComp(new _PrefabComp(__VA_ARGS__))
class PrefabItem;

CE_END_NAMESPACE

#include "prefab_objref.hpp"
#include "prefab_compref.hpp"
#include "prefab_objbase.hpp"
#include "prefab_item.hpp"
#include "prefab_obj.hpp"
#include "prefab_link.hpp"
#include "prefab_mod.hpp"
#include "prefab_comp.hpp"