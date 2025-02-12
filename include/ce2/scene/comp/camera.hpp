#include "chokoengine.hpp"
#include "camera/enums/camera_clear_type.hpp"

CE_BEGIN_NAMESPACE

class _Camera : public _Component { CE_COMPONENT_COMMON
private:
	FrameBuffer _deferredBuffer;
	bool _writeExtraBuffers;

	RenderTarget _target;
	std::array<RenderTarget, 3> _blitTargets;

	bool _orthographic;
	float _fov;
	float _orthoSize;

	float _nearClip;
	float _farClip;

	CameraClearType _clearType;
	float _clearDepth;
	Color _clearColor;

	Mat4x4 _lastViewProjectionMatrix;

	std::vector<CameraEffect> _effects;

	static bool Init();

public:
	_Camera();

	_Camera(const _Camera& rhs);

	CE_GET_MEMBER(deferredBuffer);

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

	/* The last view projection matrix of this camera,
	 * including camera transformations
	 */
	CE_GET_MEMBER(lastViewProjectionMatrix);

	/* The list of effects applied to the final render
	 * Effects are applied from front to back
	 */
	CE_GET_SET_MEMBER(effects);
	
	CE_GET_SET_MEMBER_F(writeExtraBuffers);
	
	/* Adds an effect at the back of the list
	 */
	void AddEffect(const CameraEffect&);

	Vec3 ScreenToWorldCoords(const Vec2&) const;
};

CE_END_NAMESPACE

#include "camera/effect_base.hpp"