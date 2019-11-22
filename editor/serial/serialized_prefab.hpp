#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializedPrefab : public ESerializedObject {
public:
	ESerializedPrefab(const SceneObject&);

	std::string name;
	bool enabled;
	struct {
		Vec3 position;
		Quat rotation;
		Vec3 scale;
	} transform;

	std::vector<pESerializedComponent> components;

	std::vector<pESerializedPrefab> children;

	JsonObject ToJson() const override;
};

CE_END_ED_NAMESPACE

#include "serialized_component.hpp"