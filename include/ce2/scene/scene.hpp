#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* The Scene class contains the objects and
 * details of the game world.
 */
class _Scene : public _Object { CE_OBJECT_COMMON
	std::vector<SceneObject> _objects;
	
	Background _sky;

	void DoTree(std::string& s, const std::vector<SceneObject>& o, const std::vector<bool>& level);

	SceneObject DoFindByName(const std::vector<SceneObject>& o, const std::string& nm);

	void DoUpdate(const std::vector<SceneObject>& o);
	
	void Update();

public:
	_Scene();

	CE_GET_MEMBER(objects);

	CE_GET_SET_MEMBER(sky);

	/* Adds a new sceneobject to the scene
	 */
	SceneObject AddNewObject(const SceneObject& parent = nullptr);

	/* Adds an existing sceneobject to the scene
	 */
	void AddObject(const SceneObject& o, const SceneObject& parent = nullptr);

	/* Removes a sceneobject from the scene
	 */
	void RemoveObject(const SceneObject& o);

	/* Removes all sceneobjects from the scene
	 */
	void ClearObjects();

	/* Returns the first object with the name \p nm
	 */
	SceneObject FindByName(const std::string& nm);

	/* Returns all objects satisfying the condition
	 */
	std::vector<SceneObject> FindAllByPred(std::function<bool(const SceneObject&)>);

	/* Renders all probes, cameras, and lights in the scene
	 */
	void Render();

	/* Render the selected cameras
	 */
	void RenderCameras(const std::vector<Camera>&);

	std::string Tree();
};

CE_END_NAMESPACE
