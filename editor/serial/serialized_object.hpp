#pragma once
#include "chokoeditor.hpp"

#include "serial_objectref.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializedPrefab;
typedef std::unique_ptr<ESerializedPrefab> pESerializedPrefab;
#define ESerializedPrefab_New(...) pESerializedPrefab(new ESerializedPrefab(__VA_ARGS__))

class ESerializedPrefabExt;
typedef std::unique_ptr<ESerializedPrefabExt> pESerializedPrefabExt;
#define ESerializedPrefabExt_New(...) pESerializedPrefabExt(new ESerializedPrefabExt(__VA_ARGS__))

class ESerializedPrefabMod;
typedef std::unique_ptr<ESerializedPrefabMod> pESerializedPrefabMod;
#define ESerializedPrefabMod_New(...) pESerializedPrefabMod(new ESerializedPrefabMod(__VA_ARGS__))

class ESerializedComponent;
typedef std::unique_ptr<ESerializedComponent> pESerializedComponent;
#define ESerializedComponent_New(...) pESerializedComponent(new ESerializedComponent(__VA_ARGS__))

class ESerializedItem;
typedef std::unique_ptr<ESerializedItem> pESerializedItem;
#define ESerializedItem_New(...) pESerializedItem(new ESerializedItem(__VA_ARGS__))

class ESerializedObject {
protected:
	ESerializedObject() = default;

public:
	virtual ~ESerializedObject() = default;

	std::unordered_map<std::string, ESerializedItem> items = {};

	virtual JsonObject ToJson() const;
};

typedef std::unique_ptr<ESerializedObject> pESerializedObject;

CE_END_ED_NAMESPACE

#include "serialized_item.hpp"
#include "serialized_prefab_ext.hpp"
#include "serialized_prefab.hpp"