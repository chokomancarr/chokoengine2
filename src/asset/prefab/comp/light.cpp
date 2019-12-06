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
	auto c = o->AddComponent<Light>(CE_PR_GET(type, LightType::Point));
	CE_PR_SET(strength);
	CE_PR_SET(distance);
	CE_PR_SET(radius);
	CE_PR_SET(angle);
	CE_PR_SET(color);
	CE_PR_SET(shadow);
	CE_PR_SET(shadowDistance);
	CE_PR_SET(shadowStrength);
	CE_PR_SET(shadowResolution);
	CE_PR_SET(shadowBias);
	CE_PR_SET(shadowOnly);
	CE_PR_SET(softShadows);
	CE_PR_SET(shadowSamples);
}

CE_END_NAMESPACE
