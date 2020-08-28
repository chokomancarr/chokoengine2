#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Collider::_Collider() : CE_COMPDEF(Collider) {}

_Collider::_Collider(const _Collider& r)
		: CE_COMPDEF(Collider) {
}

void _Collider::type(const ColliderType& tp) {
	_type = tp;

}

CE_END_NAMESPACE