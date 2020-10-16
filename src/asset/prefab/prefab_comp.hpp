#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

#define CE_PR_IMPL_COMP(nm) void _PrefabComp::Set ## nm (const nm& c)
#define CE_PR_IMPL_COMP_INST(nm) void _PrefabComp::Instantiate ## nm(const SceneObject& o) const
#define CE_PR_IMPL_COMP_APP(nm) void _PrefabComp::Apply ## nm(const nm& c) const

#define CE_PR_IFVALID(it) if (it != items.end())

#define CE_PR_GETI(nm) std::find_if(items.begin(), items.end(),\
	[](const std::pair<std::string, PrefabItem>& p){\
		return p.first == #nm;\
	})
template <typename V>
V _CE_PR_GET(const _PrefabObjBase* ptr, const std::string& nm, const V& vl) {
	auto it = std::find_if(ptr->items.begin(), ptr->items.end(),
		[&nm](const std::pair<std::string, PrefabItem>& p){
			return p.first == nm;
		});
	if (it == ptr->items.end()) return vl;
	else return it->second.template Get<V>();
}
#define CE_PR_GET(nm, vl) _CE_PR_GET(this, #nm, vl)

#define CE_PR_SET(nm) c->nm(CE_PR_GET(nm, c->nm()))
#define CE_PR_SET_A(nm, tp) c->nm((tp)_CE_PR_GET<Asset>(this, #nm, c->nm()))

#define CE_PR_NO_ITEMGROUP {}

#include "ce2/scene/comp/detail/compdefsall.inl"

class _PrefabComp : public _PrefabObjBase {
#define DC(nm)\
	void Set ## nm(const nm&);\
	void Instantiate ## nm(const SceneObject&) const;\
	void Apply ## nm(const nm&) const;

	COMP_DEFS_ALL(DC)

#undef CE_PR_DEF_COMP

#include "ce2/scene/comp/detail/compdefsall_undef.inl"

public:
	_PrefabComp(const Component&);

	_PrefabComp(const JsonPair&);

	ComponentType type;

	SceneObject Instantiate(const SceneObject&) override;

	void Apply(const Component&) const;
};

CE_END_NAMESPACE