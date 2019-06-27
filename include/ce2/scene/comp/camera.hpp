#include "chokoengine.hpp"
#include "enums/camera_clear_type.hpp"

CE_BEGIN_NAMESPACE

class _Camera : public _Component { CE_COMPONENT_COMMON
	FrameBuffer _deferredBuffer;

	RenderTarget _target;

	bool _orthographic;
	float _fov;
	float _orthoSize;

	float _nearClip;
	float _farClip;

	CameraClearType _clearType;
	float _clearDepth;
	Color _clearColor;

	std::vector<CameraEffect> _effects;

	static bool Init();
public:
	_Camera();

	/* The rendering target
	 * If this is null, the camera renders to the screen
	 */
	CE_GET_SET_MEMBER(target);

	/* The projection mode of the camera
	 */
	CE_GET_SET_MEMBER(orthographic);
	/* The field of view for the perspective projection
	 */
	CE_GET_SET_MEMBER(fov);
	/* The sensor size for the ortographic projection
	 */
	CE_GET_SET_MEMBER(orthoSize);
	/* The minimum visible distance
	 */
	CE_GET_SET_MEMBER(nearClip);
	/* The maximum visible distance
	 */
	CE_GET_SET_MEMBER(farClip);
	/* The information to clear before rendering
	 */
	CE_GET_SET_MEMBER(clearType);
	/* The initial depth
	 * Depth must be specified in clearType.
	 */
	CE_GET_SET_MEMBER(clearDepth);
	/* The initial color
	 * Color must be specified in clearType.
	 */
	CE_GET_SET_MEMBER(clearColor);

	/* The list of effects applied to the final render
	 * Effects are applied from front to back
	 */
	CE_GET_SET_MEMBER(effects);
	/* Adds an effect at the back of the list
	 */
	void AddEffect(const CameraEffect& e);
};

CE_END_NAMESPACE

#include "camera/effect_base.hpp"