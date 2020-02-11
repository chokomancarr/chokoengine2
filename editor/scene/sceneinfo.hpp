#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESceneInfo {

public:
	enum class SelType {
		None,
		SceneObject,
		Asset,
		String
	};
	
	static SelType selectedType;

	static SceneObject selectedObject;
	static std::vector<SceneObject> subSelectedObjects;

	static Asset selectedAsset;

	static std::string selectedString;

	static void Clear();

	template <typename T>
	static typename std::enable_if<std::is_base_of<_Object, T>::value, void>::type
	Select(const Ref<T>& o);

	static void Select(const std::string& s);
};

CE_END_ED_NAMESPACE

#include "sceneinfo.inl"