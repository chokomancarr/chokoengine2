#include "chokoengine.hpp"
#include "enums/component_types.hpp"

BEGIN_CE_NAMESPACE

class _Component { CE_OBJECT_COMMON
protected:
	_Component(ComponentType t);
public:
	virtual ~_Component() {}

	const ComponentType type;
};

END_CE_NAMESPACE

#include "comp/camera.hpp"