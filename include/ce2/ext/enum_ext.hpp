#pragma once
#include <type_traits>

#define CE_ENUM_OP_DEF(op) \
template <typename E, typename T>\
constexpr inline typename std::enable_if<std::is_enum<E>::value, E>::type operator op(E a, T b) {\
	return E((size_t)a op (size_t)b);\
}\
template <typename E, typename T>\
constexpr inline typename std::enable_if<std::is_enum<E>::value, E>::type& operator op##=(E& a, const T b) {\
	return (a = a op b);\
}

CE_ENUM_OP_DEF(&)
CE_ENUM_OP_DEF(|)
CE_ENUM_OP_DEF(^)

#undef CE_ENUM_OP_DEF

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value, E>::type*>\
constexpr inline E operator~(E a) {\
	return (size_t)a ^ ((size_t)E::_MASK);
}

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value, E>::type*>\
constexpr inline bool operator!(E a) {\
	return !(size_t)a;
}