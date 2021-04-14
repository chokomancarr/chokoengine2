#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

class Prefab_ObjRef {
public:
	Prefab_ObjRef() = default;
	Prefab_ObjRef(SceneObject tar, const SceneObject& base);
	Prefab_ObjRef(const JsonObject&);

	bool empty;
	std::vector<std::pair<std::string, int>> path;

	SceneObject Seek(const SceneObject&) const;

	template <typename T>
	T Seek(T, 
		const std::function<std::string(T)> nameof,
		const std::function<size_t(T)> numchildren,
		const std::function<T(T, size_t)> childof) const;

	JsonObject ToJson() const;
};

CE_END_NAMESPACE

#include "prefab_objref.inl"