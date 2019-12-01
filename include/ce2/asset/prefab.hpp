#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Prefab class
 * A holder of scene objects
 * Saves a configuration of objects
 * that can be instantiated into the scene
 */
class _Prefab : public _Asset { CE_OBJECT_COMMON
public:
	class _ObjBase;

private:
	std::unique_ptr<_ObjBase> _data;

public:
    _Prefab(const SceneObject&, bool link);

	_Prefab(const JsonObject&);

	JsonObject ToJson() const;

    SceneObject Instantiate() const;
};

CE_END_NAMESPACE