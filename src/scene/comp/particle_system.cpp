#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

_ParticleSystem::_ParticleSystem() : CE_COMPDEF(ParticleSystem),
		_emit(true), _emissionRate(10), _continuous(true), _prewarm(true), _timescale(1),
		_shape(ParticleEmissionShape::Sphere), _radius(1), _angle(30), _length(1),
		_initialSpeed(CRValue(1, 0)), _initialSize(CRValue(1, 0)),
		_initialRotation(CRValue(0, 0)), _initialRotSpeed(CRValue(0, 0)),
		_lifetime(1), _lifetimeSpeed(std::vector<float>({ 1.0f })), _lifetimeSize(std::vector<float>({ 1.0f })),
		_lifetimeColor({ Color(1.0f) }), 
		_maxparticles(1024), _material(nullptr),
		_useTextureTiling(false), _textureTiles(1, 1),
		_activenumpar(0), _partial_par(0) {
	_callbackMask ^= CompCallbackFlag::EXEC_IN_EDIT_MODE;
}

void _ParticleSystem::OnPostLogic() {
	Backend::Renderer::Particles::UpdateData(*this);
}

CE_END_NAMESPACE