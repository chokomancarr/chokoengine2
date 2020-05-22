#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* The Scene class contains the objects and
 * details of the game world.
 */
class _Scene : public _Object { CE_OBJECT_COMMON
	std::vector<SceneObject> _objects;
	
	Background _sky;

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

	/* Event callbacks
	 */
	void Update();
	void LateUpdate();
	void PhysicsUpdate();
	void PostLogic();
	void Paint();

	/* Explicitly renders all probes, cameras, and lights in the scene
	 */
	void Render();

	/* Explicitly renders all non-camera objects
	 * i.e. bakes probes, light shadow maps etc
	 * and updates the scene hierarchy for rendering
	 * This is called automatically by Render()
	 * but not by RenderCameras()
	 */
	void PrepareSceneForRendering();

	/* Returns the list of active cameras in the scene
	 * populated from PrepareSceneForRendering()
	 */
	std::vector<Camera> GetActiveCameras();

	/* Explicitly renders the selected cameras
	 * All cameras must be of objects attached to this scene
	 * PrepareSceneForRendering() must be called at least once
	 * every frame before this is called, or when
	 * a different scene was rendered before this
	 */
	void RenderCameras(const std::vector<Camera>&);

	/* Generates a readable string for the scene hierarchy
	 * Useful for logging to console
	 */
	std::string Tree();
};

CE_END_NAMESPACE
