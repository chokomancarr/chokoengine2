#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

_ParticleSystem::_ParticleSystem() : CE_COMPDEF(ParticleSystem),
	_emit(true), _emissionRate(10), _continuous(true), _prewarm(true),
	_lifetime(1), _lifetimeColor(Gradient::Rainbow()),
	_maxparticles(1024),
	_activenumpar(0), _partial_par(0) {}

void _ParticleSystem::OnPostLogic() {
	Backend::Renderer::Particles::UpdateData(*this);
}

CE_END_NAMESPACE