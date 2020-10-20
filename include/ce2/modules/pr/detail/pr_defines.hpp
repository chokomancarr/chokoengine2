#pragma once
#include "chokoengine.hpp"

#define CE_MOD_PR_NS ModulePR

#define CE_BEGIN_MOD_PR_NAMESPACE\
	CE_BEGIN_NAMESPACE\
	namespace CE_MOD_PR_NS {

#define CE_END_MOD_PR_NAMESPACE }\
	CE_END_NAMESPACE

#if PLATFORM_WIN
#ifdef BUILD_MODULE_PR
#define CE_PR_EXPORT __declspec(dllexport)
#else
#define CE_PR_EXPORT __declspec(dllimport)
#endif
#else
#define CE_EXPORT
#endif
