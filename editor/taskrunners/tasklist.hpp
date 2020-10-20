#pragma once
#include "chokoeditor.hpp"

/* A list of available tasks
 */

CE_BEGIN_ED_NAMESPACE

#ifdef PLATFORM_WIN
    #define CE_PROG_EXT ".exe"
#else
    #define CE_PROG_EXT 
#endif

namespace TaskList {
	const std::string builderDebug = "builder_dbg" CE_PROG_EXT;
	const std::string builderRelease = "builder_rel" CE_PROG_EXT;
}

#undef CE_PROG_EXT

CE_END_ED_NAMESPACE
