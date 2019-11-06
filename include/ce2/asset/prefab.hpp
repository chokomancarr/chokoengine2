#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Prefab class
 * A holder of scene objects
 * Saves a configuration of objects
 * that can be instantiated into the scene
 */
class _Prefab : public _Asset { CE_OBJECT_COMMON
    SceneObject _rootObject;

public:
    _Prefab(const SceneObject& o = nullptr);

    SceneObject Instantiate() const;
};

CE_END_NAMESPACE