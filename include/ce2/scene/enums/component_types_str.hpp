#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

#include "../comp/detail/compdefsall.inl"
#define MP(c) { ComponentType::c, #c },
const std::unordered_map<ComponentType, std::string> ComponentTypeStr = {
	COMP_DEFS_ALL(MP)
	{ ComponentType::_COUNT, "_COUNT" },
	{ ComponentType::Unknown, "Unknown" }
};
#undef MP
#include "../comp/detail/compdefsall_undef.inl"

CE_END_NAMESPACE
