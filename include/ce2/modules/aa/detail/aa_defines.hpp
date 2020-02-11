#pragma once
#include "chokoengine.hpp"

#define CE_MOD_AA_NS ModuleAA

#define CE_BEGIN_MOD_AA_NAMESPACE\
	CE_BEGIN_NAMESPACE\
	namespace CE_MOD_AA_NS {

#define CE_END_MOD_AA_NAMESPACE }\
	CE_END_NAMESPACE
