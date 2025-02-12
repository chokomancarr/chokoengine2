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
	class _ObjTreeBase;

	typedef std::function<Prefab(const std::string&)> _Sig2Prb;
	typedef std::function<Asset(AssetType, const std::string&)> _Sig2Ass;

private:
	std::unique_ptr<_ObjBase> _data;
	std::unique_ptr<_ObjTreeBase> _tree;

public:
    _Prefab(const SceneObject&, bool link);

	_Prefab(const JsonObject&, _Sig2Prb);

	~_Prefab(); //explicit destructor where _ObjBase is defined

	JsonObject ToJson() const;

    SceneObject Instantiate(_Sig2Ass);

	std::unique_ptr<_ObjBase>& GetPrefabObj(size_t id);

	_ObjTreeBase& GetTreeObj(size_t id);

	std::unique_ptr<_ObjTreeBase>& GetTree();

	void Apply(const SceneObject&, bool tree = false, size_t id = -1);

	void Revert(const SceneObject&, size_t id = -1);

	void _UpdateObjs(const SceneObject&);
};

CE_END_NAMESPACE