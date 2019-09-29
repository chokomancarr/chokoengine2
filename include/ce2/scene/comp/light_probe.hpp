#pragma once
#include "chokoengine.hpp"
#include "light_probe/light_probe_updatefreq.hpp"

CE_BEGIN_NAMESPACE

class _LightProbe : public _Component { CE_COMPONENT_COMMON
    float _strength;
    Vec3 _center;
    Vec3 _area;
    LightProbeUpdateFrequency _updateFrequency;
    int _resolution;
    
	FrameBufferCube _fbo;

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
     */
    CE_GET_SET_MEMBER(updateFrequency);
    /* The resolution of the computed data
     */
    CE_GET_SET_MEMBER_F(resolution);

	void Render();
};

CE_END_NAMESPACE