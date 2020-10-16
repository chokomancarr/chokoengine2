#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

#include "../comp/detail/compdefsall.inl"
#define CM(c) c,

enum class ComponentType {
	COMP_DEFS_ALL(CM)
	_COUNT,
	Unknown
};

#undef CM
#include "../comp/detail/compdefsall_undef.inl"

CE_END_NAMESPACE

#include "component_types_str.hpp"
