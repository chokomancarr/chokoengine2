#pragma once
#include "chokoengine.hpp"
#include "light_probe/light_probe_updatefreq.hpp"

CE_BEGIN_NAMESPACE

class _LightProbe : public _Component { CE_COMPONENT_COMMON
    float _strength;
    Vec3 _center;
    Vec3 _area;
    LightProbeUpdateFrequency _updateFrequency;
    bool _dirty;
    int _resolution;

    float _nearClip;
    float _farClip;

    FrameBuffer _deferredBuffer;
    
	FrameBufferCube _fbo;

    CameraClearType _clearType;

    Color _clearColor;
    float _clearDepth;

	static bool Init();
public:
    /* Constructs light probe with default settings
     */
    _LightProbe();

    /* Probe intensity
     */
    CE_GET_SET_MEMBER(strength);
    /* Position to compute the lighting,
     * relative to the object's position
     */
    CE_GET_SET_MEMBER(center);
    /* Area affected by the probe
     */
    CE_GET_SET_MEMBER(area);
    /* The frequency to recompute the lighting
     * If the dirty parameter is true, the probe
     * will be recomputed regardless of this value
     */
    CE_GET_SET_MEMBER(updateFrequency);
    /* Should the probe be recomputed on the next frame
     * This flag overrides the updateFrequency parameter
     * This flag is reset to false when recomputed
     */
    CE_GET_SET_MEMBER(dirty);
    /* The resolution of the computed data
     */
    CE_GET_SET_MEMBER_F(resolution);

    const CubeMap& result();

	void Render();
};

CE_END_NAMESPACE