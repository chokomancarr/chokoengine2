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

	RenderTarget _shadowTarget;

    bool _shadow;
    float _shadowDistance;
    float _shadowStrength;
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
     * Has no effect on non-directional models
     */
    CE_GET_SET_MEMBER(distance);
    /* Radius of the center where source has maximum intensity
     * Has no effect on direcional models
     */
    CE_GET_SET_MEMBER(radius);

    /* Does the light casts shadow
	 */
	CE_GET_SET_MEMBER(shadow);
    /* Maximum distance to evaluate distance
     * Lower values give better accuracy
     */
    CE_GET_SET_MEMBER(shadowDistance);
    /* Strength of the shadow
     */
    CE_GET_SET_MEMBER(shadowStrength);
    /* Scene is darkened in shadowed areas
     * shadowStrength can be > 1 in this case
     */
    CE_GET_SET_MEMBER(shadowOnly);
};

CE_END_NAMESPACE

#include "camera/effect_base.hpp"