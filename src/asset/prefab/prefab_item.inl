#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

template <typename T, typename std::enable_if<
	std::is_enum<T>::value && !std::is_same<T, PrefabItem::Type>::value, T>::type*>
PrefabItem::PrefabItem(const T& e, const std::string& nm) : PrefabItem(Type::Int, nm) {
	value.i = (int)e;
}

#define ISENUM std::is_enum<T>::value
#define ISASSET is_base_ref_of<Asset, T>::value
#define ISCOMP is_base_ref_of<Component, T>::value

template <typename T> typename std::enable_if<ISENUM, T>::type
PrefabItem::Get() const {
	return (T)value.i;
}

template <typename T> typename std::enable_if<ISASSET, T>::type
PrefabItem::Get() const {
	return (T)_Get<Asset>();
}

template <typename T> typename std::enable_if<ISCOMP, T>::type
PrefabItem::Get() const {
	return (T)_Get<Component>();
}

template <typename T> typename std::enable_if<!ISENUM && !ISASSET && !ISCOMP, T>::type
PrefabItem::Get() const {
	return _Get<T>();
}

//not defined
//template <typename T>
//T PrefabItem::_Get() const;

#define CE_ES_SPEC_F(tp) template <>\
	inline tp PrefabItem::_Get<tp>() const
#define CE_ES_SPEC(tp, var) CE_ES_SPEC_F(tp) {\
		return (tp)value.var;\
	}

CE_ES_SPEC(bool, i)
CE_ES_SPEC(float, f)
CE_ES_SPEC(int, i)
CE_ES_SPEC(Vec2, v2)
CE_ES_SPEC(Vec3, v3)
CE_ES_SPEC(Vec4, v4)
CE_ES_SPEC(Quat, q)
CE_ES_SPEC(std::string, s)
CE_ES_SPEC(Color, v4)

CE_ES_SPEC_F(Asset) {
	auto& av = value.assetref;
	return PrefabState::sig2AssFn(av.assetType, av.sig);
}
CE_ES_SPEC_F(const PrefabItemGroup&) {
	return value.group;
}
CE_ES_SPEC_F(const PrefabObjGroup&) {
	return value.objgroup;
}
CE_ES_SPEC_F(Prefab_ObjRef) {
	return value.scobjref;
}
CE_ES_SPEC_F(Prefab_CompRef) {
	return value.compref;
}

CE_ES_SPEC_F(CRValue) {
	CRValue res = {};
	for (auto& g : value.group) {
		if (g.name == "C") {
			res.constant = g.value.f;
		}
		else if (g.name == "R") {
			res.random = g.value.f;
		}
	}
	return res;
}

CE_ES_SPEC_F(Gradient) {
	Gradient res = {};
	for (auto& g : value.group) {
		res.entries.push_back(Gradient::Entry(StrExt::ToFloat(g.name), g.Get<Color>()));
	}
	return res;
}

CE_END_NAMESPACE