#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

template <typename T, typename std::enable_if<std::is_enum<T>::value, T>::type*>
PrefabItem::PrefabItem(const T& e) : value({}), type(Type::Int) {
	value.i = (int)e;
}

template <typename T> typename std::enable_if<std::is_enum<T>::value, T>::type
PrefabItem::Get() const {
	return (T)value.i;
}

template <typename T> typename std::enable_if<!std::is_enum<T>::value, T>::type
PrefabItem::Get() const {
	return _Get<T>();
}

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
CE_ES_SPEC(Color, v4)

/*
CE_ES_SPEC_F(Asset) {
	auto& av = value.assetref;
	return EAssetList::Get(av.assetType, av.sig, true);
}
*/

CE_END_NAMESPACE