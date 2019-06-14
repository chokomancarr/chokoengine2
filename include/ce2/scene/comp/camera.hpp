#include "chokoengine.hpp"
#include "enums/camera_clear_type.hpp"

CE_BEGIN_NAMESPACE

class _Camera : public _Component { CE_COMPONENT_COMMON
	bool _orthographic;
	float _fov;
	float _orthoSize;

	float _nearClip;
	float _farClip;

	CameraClearType _clearType;
	float _clearDepth;
	Color _clearColor;

	static bool Init();
public:
	_Camera();

	CE_GET_SET_MEMBER(orthographic);
	CE_GET_SET_MEMBER(fov);
	CE_GET_SET_MEMBER(orthoSize);
	CE_GET_SET_MEMBER(nearClip);
	CE_GET_SET_MEMBER(farClip);
	CE_GET_SET_MEMBER(clearType);
	CE_GET_SET_MEMBER(clearDepth);
	CE_GET_SET_MEMBER(clearColor);
};

CE_END_NAMESPACE