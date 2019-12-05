#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

#define CE_PR_IMPL_COMP(nm) void _PrefabComp::Set ## nm (const nm& c)
#define CE_PR_IMPL_COMP_INST(nm) void _PrefabComp::Instantiate ## nm(const SceneObject& o) const

#define CE_PR_GETI(nm) std::find_if(items.begin(), items.end(),\
	[](const std::pair<std::string, PrefabItem>& p){\
		return p.first == #nm;\
	})->second
#define CE_PR_GET(nm, tp) CE_PR_GETI(nm).Get<tp>()

#define CE_PR_SET(nm, tp) c->nm(CE_PR_GET(nm, tp))
#define CE_PR_SET_A(nm, tp) c->nm((tp)CE_PR_GET(nm, Asset))

/* We should change this to class template if possible
 */
class _PrefabComp : public _PrefabObjBase {
#define CE_PR_DEF_COMP(nm)\
	void Set ## nm(const nm&);\
	void Instantiate ## nm(const SceneObject&) const;

	CE_PR_DEF_COMP(Animator)
	CE_PR_DEF_COMP(Camera)
	CE_PR_DEF_COMP(Light)
	CE_PR_DEF_COMP(MeshRenderer)
	CE_PR_DEF_COMP(Rig)
#undef CE_PR_DEF_COMP

public:
	_PrefabComp(const Component&);

	_PrefabComp(const JsonObject&);

	ComponentType type;

	bool enabled;

	JsonPair ToJson() const override;

	SceneObject Instantiate(const SceneObject&) const override;
};

CE_END_NAMESPACE