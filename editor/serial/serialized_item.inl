#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T, typename std::enable_if<std::is_enum<T>::value, T>::type*>
ESerializedItem::ESerializedItem(const T& e) : value({}), type(Type::Int) {
	value.i = (int)e;
}

CE_END_ED_NAMESPACE