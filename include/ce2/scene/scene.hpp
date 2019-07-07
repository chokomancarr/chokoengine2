#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Scene { CE_CLASS_COMMON
	static CE_EXPORT std::vector<SceneObject> _objects;
	
	static CE_EXPORT Background _sky;

	static void DoTree(std::string& s, const std::vector<SceneObject>& o, const std::vector<bool>& level);

	static SceneObject DoFindByName(const std::vector<SceneObject>& o, const std::string& nm); 

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

	/* Returns the first object with the name \p nm
	 */
	static SceneObject FindByName(const std::string& nm);

	static std::string Tree();
};

CE_END_NAMESPACE