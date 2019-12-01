#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class _Prefab::_ObjBase {
protected:
	_ObjBase() = default;

	_ObjBase(const JsonObject&);

public:
	virtual ~_ObjBase() = default;

	std::unordered_map<std::string, PrefabItem> items = {};

	virtual JsonObject ToJson() const;

	virtual SceneObject Instantiate(const SceneObject&) const = 0;
};

CE_END_NAMESPACE