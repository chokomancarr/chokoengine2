#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Collider::_Collider() : CE_COMPDEF(Collider), _type(ColliderType::Sphere),
	_radius(1), _length(10), _size(1), _mesh(nullptr), _convex(false) {}

_Collider::_Collider(const _Collider& r)
		: CE_COMPDEF(Collider) {
}

void _Collider::OnStart() {
	Physics::OnBodyAdded(object());
}

void _Collider::type(const ColliderType& tp) {
	_type = tp;

}

CE_END_NAMESPACE