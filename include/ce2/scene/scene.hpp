#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Scene : public _Object { CE_CLASS_COMMON
	std::vector<SceneObject> _objects;
	
	Background _sky;

	static void DoTree(std::string& s, const std::vector<SceneObject>& o, const std::vector<bool>& level);

public:
	_Scene(const std::string& nm = "");

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

	std::string Tree() const;
};

CE_END_NAMESPACE