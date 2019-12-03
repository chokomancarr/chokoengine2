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

	typedef std::function<Prefab(const std::string&)> _Sig2Prb;
	typedef std::function<Asset(AssetType, const std::string&)> _Sig2Ass;

private:
	std::unique_ptr<_ObjBase> _data;

public:
    _Prefab(const SceneObject&, bool link);

	_Prefab(const JsonObject&, _Sig2Prb);

	JsonObject ToJson() const;

    SceneObject Instantiate(_Sig2Ass) const;
};

CE_END_NAMESPACE