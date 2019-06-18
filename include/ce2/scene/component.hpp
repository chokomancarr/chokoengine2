#include "chokoengine.hpp"
#include "enums/component_types.hpp"

CE_BEGIN_NAMESPACE

class _Component : public Object { CE_OBJECT_COMMON
protected:
	_Component(ComponentType t, const std::string& nm);

	pSceneObject _object;
public:
	virtual ~_Component() = default;

	const ComponentType type;

	SceneObject object();

	friend class _SceneObject; //allow set _object
};

CE_END_NAMESPACE

#define CE_COMPDEF(nm) _Component(ComponentType::nm, #nm)

#include "comp/camera.hpp"