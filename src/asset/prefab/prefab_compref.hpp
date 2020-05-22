#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class Prefab_CompRef {
public:
	Prefab_CompRef() = default;
	Prefab_CompRef(const Component& tar, const SceneObject& base);
	Prefab_CompRef(const JsonObject&);

	Prefab_ObjRef objref;
	ComponentType type;

	Component Seek(const std::vector<SceneObject>&) const;

	JsonObject ToJson() const;
};

CE_END_NAMESPACE