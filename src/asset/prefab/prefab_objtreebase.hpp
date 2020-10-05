#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _Prefab::_ObjTreeBase {
public:
	_ObjTreeBase() = default;

	std::string name;
	_ObjBase* obj;
	std::vector<_PrefabMod*> mods;
	bool indirect;

	std::vector<_ObjTreeBase> children;
};

CE_END_NAMESPACE