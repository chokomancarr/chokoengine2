#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Scene { CE_CLASS_COMMON
	std::vector<SceneObject> objects;
	
public:
	/* Adds a new sceneobject to the scene
	 */
	SceneObject AddNewObject();

	/* Adds an existing sceneobject to the scene
	 */
	void AddObject(const SceneObject& o);

	/* Removes a sceneobject from the scene
	 */
	void RemoveObject(const SceneObject& o);
};

CE_END_NAMESPACE