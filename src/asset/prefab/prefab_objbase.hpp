#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

#define CE_PR_ADDV(nm, vl) items.push_back(std::make_pair(#nm, PrefabItem(vl)))
#define CE_PR_ADD(nm) CE_PR_ADDV(nm, c->nm())

#define CE_PR_ADDGROUP(nm) (items.push_back(std::make_pair(#nm, PrefabItem::Type::ObjGroup)),\
	items.back().second.value.objgroup)

class _Prefab::_ObjBase {
public:
	enum class Type {
		Object,
		Link,
		Comp
	};

protected:
	_ObjBase(Type t);

	_ObjBase(Type t, const JsonObject&);

	std::string name;

public:
	virtual ~_ObjBase() = default;

	const Type _type;

	std::vector<std::pair<std::string, PrefabItem>> items = {};

	std::vector<PrefabMod> mods = {};

	virtual JsonPair ToJson() const;

	virtual void LoadLinks() {}

	virtual SceneObject Instantiate(const SceneObject&) = 0;
};

CE_END_NAMESPACE