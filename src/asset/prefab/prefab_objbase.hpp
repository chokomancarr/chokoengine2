#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

#define CE_PR_ADDV(nm, vl) items.push_back(std::make_pair(#nm, PrefabItem(vl)))
#define CE_PR_ADD(nm) CE_PR_ADDV(nm, c->nm())

#define CE_PR_ADDGROUP(nm) (items.push_back(std::make_pair(#nm, PrefabItem::Type::ObjGroup)),\
	items.back().second.value.objgroup)

class _Prefab::_ObjBase {
protected:
	_ObjBase() = default;

	_ObjBase(const JsonObject&);

	std::string name;

public:
	virtual ~_ObjBase() = default;

	std::vector<std::pair<std::string, PrefabItem>> items = {};

	virtual JsonPair ToJson() const;

	virtual SceneObject Instantiate(const SceneObject&) const = 0;
};

CE_END_NAMESPACE