#pragma once
#include "chokoengine.hpp"

#define CE_MOD_AE_NS ModuleAE

#define CE_BEGIN_MOD_AE_NAMESPACE\
	CE_BEGIN_NAMESPACE\
	namespace CE_MOD_AE_NS {

#define CE_END_MOD_AE_NAMESPACE }\
	CE_END_NAMESPACE

#if PLATFORM_WIN
#ifdef BUILD_MODULE_AE
#define CE_AE_EXPORT __declspec(dllexport)
#else
#define CE_AE_EXPORT __declspec(dllimport)
#endif
#else
#define CE_EXPORT
#endif
