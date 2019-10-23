#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Prefab class
 * A holder of scene objects
 * Saves a configuration of objects,
 * that can be instantiated into the scene
 */
class _Prefab : public _Asset { CE_OBJECT_COMMON

    std::vector<SceneObject> _objects;

public:
    _Prefab(const std::vector<SceneObject>& o = {});

    void AddObject(const SceneObject& o);

    std::vector<SceneObject> Instantiate();
};

CE_END_NAMESPACE