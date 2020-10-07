#pragma once

#define CE_VERSION_MAJOR 2
#define CE_VERSION_MINOR 0
#define CE_VERSION_REVISION 0

#define CE_NAMESPACE ChokoEngine
#define CE_NS ::CE_NAMESPACE::

#define CE_BEGIN_NAMESPACE namespace CE_NAMESPACE {
#define CE_END_NAMESPACE }

#define CE_NOT_IMPLEMENTED\
    throw std::logic_error("Function not implemented!");

#define CE_STRINGIFY(x) CE_STRINGIFY2(x)
#define CE_STRINGIFY2(x) #x

#define CE_ABORT()\
	do {\
		std::cerr << "process aborted at " __FILE__ ":" CE_STRINGIFY(__LINE__) "!" << std::endl;\
		abort();\
	} while (0)

#ifdef _MSC_VER
	#pragma warning(disable:4996)
#endif

#if PLATFORM_WIN
	#ifdef BUILD_CHOKOENGINE
		#define CE_EXPORT __declspec(dllexport)
	#else
		#define CE_EXPORT __declspec(dllimport)
	#endif
#else
	#define CE_EXPORT
#endif

#include "defines_object.hpp"
#include "defines_script.hpp"