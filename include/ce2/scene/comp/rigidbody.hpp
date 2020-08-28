#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Rigidbody : public _Component { CE_COMPONENT_COMMON
	float _mass;
	float _moment;

	float _bounce;

	Vec3 _velocity;

	Quat _angularVelocity;

	Vec3 _acceleration;

	Quat _torque;

public:
	_Rigidbody();
	_Rigidbody(const _Rigidbody&) = default;

	void OnStart() override;

	CE_GET_SET_MEMBER_F(mass);
	CE_GET_SET_MEMBER_F(moment);
	CE_GET_SET_MEMBER_F(bounce);
	CE_GET_SET_MEMBER_F(velocity);
	CE_GET_SET_MEMBER_F(angularVelocity);
	CE_GET_SET_MEMBER_F(acceleration);
	CE_GET_SET_MEMBER_F(torque);
};

CE_END_NAMESPACE