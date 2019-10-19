#pragma once
#include "chokoeditor.hpp"
#include "script_var.hpp"

CE_BEGIN_ED_NAMESPACE

class ScriptInfo {
public:
	std::string name;
	std::string className;

	std::vector<ScriptTypeExt> types;
	std::vector<ScriptVar> vars;
};

CE_END_ED_NAMESPACE