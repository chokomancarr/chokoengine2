#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Light) {
	CE_PR_ADD(type);
	CE_PR_ADD(falloff);
	CE_PR_ADD(strength);
	CE_PR_ADD(distance);
	CE_PR_ADD(radius);
	CE_PR_ADD(angle);
	CE_PR_ADD(color);
	CE_PR_ADD(shadow);
	CE_PR_ADD(shadowDistance);
	CE_PR_ADD(shadowStrength);
	CE_PR_ADD(shadowResolution);
	CE_PR_ADD(shadowBias);
	CE_PR_ADD(shadowOnly);
	CE_PR_ADD(softShadows);
	CE_PR_ADD(shadowSamples);
}

CE_PR_IMPL_COMP_INST(Light) {
	auto c = o->AddComponent<Light>(CE_PR_GET(type, LightType));
	CE_PR_SET(strength, float);
	CE_PR_SET(distance, float);
	CE_PR_SET(radius, float);
	CE_PR_SET(angle, float);
	CE_PR_SET(color, Color);
	CE_PR_SET(shadow, bool);
	CE_PR_SET(shadowDistance, float);
	CE_PR_SET(shadowStrength, float);
	CE_PR_SET(shadowResolution, int);
	CE_PR_SET(shadowBias, float);
	CE_PR_SET(shadowOnly, bool);
	CE_PR_SET(softShadows, bool);
	CE_PR_SET(shadowSamples, int);
}

CE_END_NAMESPACE
