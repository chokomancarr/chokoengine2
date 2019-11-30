#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class CE_S_ObjectRef {
public:
	CE_S_ObjectRef() = default;
	CE_S_ObjectRef(SceneObject tar, const SceneObject& base);
	CE_S_ObjectRef(const JsonObject&);

	std::vector<std::pair<std::string, int>> path;

	const SceneObject& Seek(const std::vector<SceneObject>&);

	JsonObject ToJson() const;
};

CE_END_ED_NAMESPACE