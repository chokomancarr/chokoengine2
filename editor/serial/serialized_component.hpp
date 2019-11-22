#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_ES_IMPL_COMP(nm) void ESerializedComponent::Set ## nm (const nm& c)
#define CE_ES_ADD(nm) items.insert(std::make_pair(#nm, c->nm()))

class ESerializedComponent : public ESerializedObject {
#define CE_ES_DEF_COMP(nm) void Set ## nm(const nm&);
	CE_ES_DEF_COMP(Animator)
	CE_ES_DEF_COMP(Camera)
	CE_ES_DEF_COMP(Light)
	CE_ES_DEF_COMP(MeshRenderer)
	CE_ES_DEF_COMP(Rig)
#undef CE_ES_DEF_COMP

public:
	ESerializedComponent(const Component&);

	ComponentType type;

	bool enabled;

	JsonObject ToJson() const override;
};

CE_END_ED_NAMESPACE