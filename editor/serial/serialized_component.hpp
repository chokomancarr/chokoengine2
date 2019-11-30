#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_ES_IMPL_COMP(nm) void ESerializedComponent::Set ## nm (const nm& c)
#define CE_ES_IMPL_COMP_INST(nm) void ESerializedComponent::Instantiate ## nm(const SceneObject& o) const

#define CE_ES_ADD(nm) items.insert(std::make_pair(#nm, c->nm()))

#define CE_ES_SET(nm, tp) c->nm(items.at(#nm).Get<tp>())
#define CE_ES_SET_A(nm, tp) c->nm((tp)items.at(#nm).Get<Asset>())

/* We should change this to class template if possible
 */
class ESerializedComponent : public ESerializedObject {
#define CE_ES_DEF_COMP(nm)\
	void Set ## nm(const nm&);\
	void Instantiate ## nm(const SceneObject&) const;
	
	CE_ES_DEF_COMP(Animator)
	CE_ES_DEF_COMP(Camera)
	CE_ES_DEF_COMP(Light)
	CE_ES_DEF_COMP(MeshRenderer)
	CE_ES_DEF_COMP(Rig)
#undef CE_ES_DEF_COMP

public:
	ESerializedComponent(const Component&);

	ESerializedComponent(const JsonObject&);

	ComponentType type;

	bool enabled;

	JsonObject ToJson() const override;

	SceneObject Instantiate(const SceneObject&) const override;
};

CE_END_ED_NAMESPACE