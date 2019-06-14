#include "chokoengine.hpp"
#include "enums/component_types.hpp"

CE_BEGIN_NAMESPACE

class _Component { CE_OBJECT_COMMON
protected:
	_Component(ComponentType t);
public:
	virtual ~_Component() {}

	const ComponentType type;
};

CE_END_NAMESPACE

#include "comp/camera.hpp"