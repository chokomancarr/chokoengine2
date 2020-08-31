#pragma once
#include "chokoplayer.hpp"
#include "script_var_entry.hpp"

CE_BEGIN_PL_NAMESPACE

/* struct describing a script class variable,
 * used for serialization
 */

struct ScriptClassEntry {
    std::string name;

	std::vector<ScriptVarEntry> vars;
};

CE_END_PL_NAMESPACE
