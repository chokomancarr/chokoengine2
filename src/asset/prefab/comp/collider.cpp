#include "../prefab_inc.hpp"

CE_BEGIN_NAMESPACE

CE_PR_IMPL_COMP(Collider) {
	CE_PR_ADD(type);
	CE_PR_ADD(radius);
}

CE_PR_IMPL_COMP_INST(Collider) {
	auto c = o->AddComponent<Collider>();
	c->type(CE_PR_GET(type, ColliderType::Sphere));
	CE_PR_SET(radius);
}

CE_END_NAMESPACE
