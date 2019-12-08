#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class Prefab_ObjRef {
public:
	Prefab_ObjRef() = default;
	Prefab_ObjRef(SceneObject tar, const SceneObject& base);
	Prefab_ObjRef(const JsonObject&);

	std::vector<std::pair<std::string, int>> path;

	const SceneObject& Seek(const std::vector<SceneObject>&) const;

	JsonObject ToJson() const;
};

CE_END_NAMESPACE