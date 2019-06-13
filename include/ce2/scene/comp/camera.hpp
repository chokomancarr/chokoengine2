#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class _Camera : public _Component { CE_COMPONENT_COMMON

	static bool Init();
public:
	_Camera();
};

END_CE_NAMESPACE