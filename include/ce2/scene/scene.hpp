#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Scene { CE_CLASS_COMMON
	static std::vector<SceneObject> _objects;
	
	static Background _sky;

	static void DoTree(std::string& s, const std::vector<SceneObject>& o, const std::vector<bool>& level);

public:
	CE_GET_ST_MEMBER(objects);

	CE_GET_SET_ST_MEMBER(sky);

	/* Adds a new sceneobject to the scene
	 */
	static SceneObject AddNewObject(const SceneObject& parent = nullptr);

	/* Adds an existing sceneobject to the scene
	 */
	static void AddObject(const SceneObject& o, const SceneObject& parent = nullptr);

	/* Removes a sceneobject from the scene
	 */
	static void RemoveObject(const SceneObject& o);

	/* Removes all sceneobjects from the scene
	 */
	static void ClearObjects();

	static std::string Tree();
};

CE_END_NAMESPACE