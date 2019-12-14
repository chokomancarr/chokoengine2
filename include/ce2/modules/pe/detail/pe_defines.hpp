#pragma once
#include "chokoengine.hpp"

#define CE_MOD_PE_NS ModulePE

#define CE_BEGIN_MOD_PE_NAMESPACE\
	CE_BEGIN_NAMESPACE\
	namespace CE_MOD_PE_NS {

#define CE_END_MOD_PE_NAMESPACE }\
	CE_END_NAMESPACE

#if PLATFORM_WIN
#ifdef BUILD_MODULE_PE
#define CE_PE_EXPORT __declspec(dllexport)
#else
#define CE_PE_EXPORT __declspec(dllimport)
#endif
#else
#define CE_EXPORT
#endif
