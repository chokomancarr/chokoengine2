#pragma once
#include "chokoengine.hpp"
#include "light/light_type.hpp"
#include "light/light_falloff.hpp"

CE_BEGIN_NAMESPACE

class _Light : public _Component { CE_COMPONENT_COMMON
    LightType _type;
    LightFalloff _falloff;

    float _strength;
    float _distance;
    float _radius;
	float _angle;
	Color _color;

	FrameBuffer shadowBuffer_2D;

    bool _shadow;
    float _shadowDistance;
    float _shadowStrength;
	float _shadowBias;
	int _shadowResolution;
    bool _shadowOnly;

	static bool Init();
public:
    /* Constructs light of specified type with default settings
     */
    _Light(LightType);

    /* Type of the light model
     */
    CE_GET_SET_MEMBER(type);
    /* Intensity falloff model
     * Has no effect on directional models
     */
    CE_GET_SET_MEMBER(falloff);

    /* Light intensity
     */
    CE_GET_SET_MEMBER(strength);
    /* Maximum distance of light reach
     * Has no effect on directional models
     */
    CE_GET_SET_MEMBER(distance);
    /* Radius of the center where source has maximum intensity
     * Has no effect on directional models
     */
    CE_GET_SET_MEMBER(radius);
	/* Angle of the light cone
	 * Only applies to spot models
	 */
	CE_GET_SET_MEMBER(angle);
	/* Color of the light
	 */
	CE_GET_SET_MEMBER(color);

    /* Does the light cast shadows
	 */
	CE_GET_SET_MEMBER_F(shadow);
    /* Maximum distance to evaluate distance
     * Lower values give better accuracy
     */
    CE_GET_SET_MEMBER(shadowDistance);
    /* Strength of the shadow
     */
    CE_GET_SET_MEMBER(shadowStrength);
    /* Resolution of the shadow map
	 * A PoT texture is recommended
     */
	CE_GET_SET_MEMBER_F(shadowResolution);
	/* Linear bias of the shadow map
	 * Larger values prevent false shadowing on surfaces
	 */
	CE_GET_SET_MEMBER(shadowBias);
	/* Scene is darkened in shadowed areas
	 * shadowStrength can be > 1 in this case
	 */
    CE_GET_SET_MEMBER(shadowOnly);
};

CE_END_NAMESPACE

#include "camera/effect_base.hpp"