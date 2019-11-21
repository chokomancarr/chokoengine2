#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializedPrefab;
typedef std::unique_ptr<ESerializedPrefab> pESerializedPrefab;

class ESerializedComponent;
typedef std::unique_ptr<ESerializedComponent> pESerializedComponent;

class ESerializedItem;
typedef std::unique_ptr<ESerializedItem> pESerializedItem;

class ESerializedObject {
protected:
	ESerializedObject() = default;

public:
	virtual ~ESerializedObject() = default;

	std::unordered_map<std::string, ESerializedItem> items = {};
};

typedef std::unique_ptr<ESerializedObject> pESerializedObject;

CE_END_ED_NAMESPACE

#include "serialized_item.hpp"
#include "serialized_prefab.hpp"