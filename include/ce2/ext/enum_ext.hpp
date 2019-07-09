#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Defines flag operations for an enum
 * The _MASK flag must be defined
 */
#define CE_DEF_ENUM_OP_FLAGS(T)\
constexpr inline T operator~ (const T a) { return (T)(~(size_t)a & ((size_t)T::_MASK - 1); }\
constexpr inline T operator| (const T a, const T b) { return (T)((size_t)a | (size_t)b); }\
constexpr inline T operator& (const T a, const T b) { return (T)((size_t)a & (size_t)b); }\
constexpr inline T operator^ (const T a, const T b) { return (T)((size_t)a ^ (size_t)b); }\
inline T& operator|= (T& a, const T b) { return (T&)((size_t&)a |= (size_t)b); }\
inline T& operator&= (T& a, const T b) { return (T&)((size_t&)a &= (size_t)b); }\
inline T& operator^= (T& a, const T b) { return (T&)((size_t&)a ^= (size_t)b); }

CE_END_NAMESPACE