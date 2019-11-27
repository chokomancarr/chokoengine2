#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_ES_IMPL_COMP(Light) {
	CE_ES_ADD(type);
	CE_ES_ADD(falloff);
	CE_ES_ADD(strength);
	CE_ES_ADD(distance);
	CE_ES_ADD(radius);
	CE_ES_ADD(angle);
	CE_ES_ADD(color);
	CE_ES_ADD(shadow);
	CE_ES_ADD(shadowDistance);
	CE_ES_ADD(shadowStrength);
	CE_ES_ADD(shadowResolution);
	CE_ES_ADD(shadowBias);
	CE_ES_ADD(shadowOnly);
	CE_ES_ADD(softShadows);
	CE_ES_ADD(shadowSamples);
}

CE_ES_IMPL_COMP_INST(Light) {
	auto c = o->AddComponent<Light>(items.at("type").Get<LightType>());
	CE_ES_SET(strength, float);
	CE_ES_SET(distance, float);
	CE_ES_SET(radius, float);
	CE_ES_SET(angle, float);
	CE_ES_SET(color, Color);
	CE_ES_SET(shadow, bool);
	CE_ES_SET(shadowDistance, float);
	CE_ES_SET(shadowStrength, float);
	CE_ES_SET(shadowResolution, int);
	CE_ES_SET(shadowBias, float);
	CE_ES_SET(shadowOnly, bool);
	CE_ES_SET(softShadows, bool);
	CE_ES_SET(shadowSamples, int);
}

CE_END_ED_NAMESPACE
