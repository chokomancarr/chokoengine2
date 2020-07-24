#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _ParticleSystem : public _Component { CE_COMPONENT_COMMON
	bool _emit;

	float _emissionRate;

	bool _continuous;
	bool _prewarm;

	float _timescale;

	CRValue _initialSpeed;
	CRValue _initialSize;
	CRValue _initialRotation;
	CRValue _initialRotSpeed;

	CRValue _lifetime;

	FCurve _lifetimeSpeed;
	FCurve _lifetimeSize;
	
	Gradient _lifetimeColor;

	Vec3 _force, _randomForce;

	Vec3 _torque, _randomTorque;

	int _maxparticles;

	Material _material;

	bool _useTextureTiling;
	Int2 _textureTiles;

public:
	_ParticleSystem();

	_ParticleSystem(const _ParticleSystem& rhs) = default;

	/* Is the emitter active
	 */
	CE_GET_SET_MEMBER(emit);

	CE_GET_SET_MEMBER(emissionRate);

	/* constantly emit particles over time
	 */
	CE_GET_SET_MEMBER(continuous);
	/* precalculate emission in negative time
	 */
	CE_GET_SET_MEMBER(prewarm);

	/* Simulation speed relative to game speed
	 */
	CE_GET_SET_MEMBER(timescale);

	CE_GET_SET_MEMBER(initialSpeed);
	CE_GET_SET_MEMBER(initialSize);
	CE_GET_SET_MEMBER(initialRotation);
	CE_GET_SET_MEMBER(initialRotSpeed);

	CE_GET_SET_MEMBER(lifetime);

	CE_GET_SET_MEMBER_MUT(lifetimeSpeed);
	CE_GET_SET_MEMBER_MUT(lifetimeSize);

	CE_GET_SET_MEMBER_MUT(lifetimeColor);

	CE_GET_SET_MEMBER(force);
	CE_GET_SET_MEMBER(randomForce);

	CE_GET_SET_MEMBER(torque);
	CE_GET_SET_MEMBER(randomTorque);

	CE_GET_SET_MEMBER(material);

	CE_GET_SET_MEMBER(useTextureTiling);
	CE_GET_SET_MEMBER(textureTiles);

	void OnPostLogic() override;

private:
	int _activenumpar;

	float _partial_par;

	std::array<VertexArray, 2> _data;
	VertexArray _mesh;
};

CE_END_NAMESPACE