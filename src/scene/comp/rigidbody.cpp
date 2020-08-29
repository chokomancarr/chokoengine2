#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Rigidbody::_Rigidbody() : CE_COMPDEF(Rigidbody), _dynamic(true), _mass(1),
	_moment(1), _bounce(0.8f), _velocity(0), _angularVelocity(Quat::identity()),
	_acceleration(0), _torque(Quat::identity()) {}


void _Rigidbody::OnStart() {
	Physics::OnBodyAdded(object());
}

#define SET(nm) \
void _Rigidbody::nm(const decltype(_ ## nm)& v) {\
	_ ## nm = v;\
	Physics::OnBodyChanged(object());\
}

void _Rigidbody::dynamic(const bool& v) {
	_dynamic = v;
	if (!_dynamic) {
		_velocity = 0;
		_angularVelocity = Quat::identity();
		_acceleration = 0;
		_torque = Quat::identity();
	}
	Physics::OnBodyChanged(object());
}
SET(mass);
SET(moment);
SET(bounce);
SET(velocity);
SET(angularVelocity);
SET(acceleration);
SET(torque);

CE_END_NAMESPACE