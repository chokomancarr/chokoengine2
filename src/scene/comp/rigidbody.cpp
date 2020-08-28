#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Rigidbody::_Rigidbody() : CE_COMPDEF(Rigidbody) {}


void _Rigidbody::OnStart() {
	Physics::OnBodyAdded(object());
}

#define SET(nm) \
void _Rigidbody::nm(const decltype(_ ## nm)& v) {\
	_ ## nm = v;\
	Physics::OnBodyChanged(object());\
}

SET(mass);
SET(moment);
SET(bounce);
SET(velocity);
SET(angularVelocity);
SET(acceleration);
SET(torque);

CE_END_NAMESPACE