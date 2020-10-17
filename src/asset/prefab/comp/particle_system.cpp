#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(ParticleSystem) {
	//CE_PR_ADD
	CE_PR_ADD(emit);
	CE_PR_ADD(emissionRate);
	CE_PR_ADD(continuous);
	CE_PR_ADD(prewarm);
	CE_PR_ADD(timescale);

	CE_PR_ADD(shape);
	CE_PR_ADD(radius);
	CE_PR_ADD(angle);
	CE_PR_ADD(length);

	CE_PR_ADD(initialSpeed);
	CE_PR_ADD(initialSize);
	CE_PR_ADD(initialRotation);
	CE_PR_ADD(initialRotSpeed);
	CE_PR_ADD(lifetime);

	CE_PR_ADD(lifetimeColor);
	
	CE_PR_ADD(force);
	CE_PR_ADD(randomForce);
	CE_PR_ADD(torque);
	CE_PR_ADD(randomTorque);
	CE_PR_ADD(material);
	CE_PR_ADD(useTextureTiling);
	//CE_PR_ADD(textureTiles);
}

CE_PR_IMPL_COMP_INST(ParticleSystem) {
	auto c = o->AddComponent<ParticleSystem>();

	ApplyParticleSystem(c);
}

CE_PR_IMPL_COMP_APP(ParticleSystem) {
	CE_PR_SET(emit);
	CE_PR_SET(emissionRate);
	CE_PR_SET(continuous);
	CE_PR_SET(prewarm);
	CE_PR_SET(timescale);

	CE_PR_SET(shape);
	CE_PR_SET(radius);
	CE_PR_SET(angle);
	CE_PR_SET(length);

	CE_PR_SET(initialSpeed);
	CE_PR_SET(initialSize);
	CE_PR_SET(initialRotation);
	CE_PR_SET(initialRotSpeed);
	CE_PR_SET(lifetime);

	CE_PR_SET(lifetimeColor);

	CE_PR_SET(force);
	CE_PR_SET(randomForce);
	CE_PR_SET(torque);
	CE_PR_SET(randomTorque);
	CE_PR_SET(material);
	CE_PR_SET(useTextureTiling);
}

CE_END_NAMESPACE
