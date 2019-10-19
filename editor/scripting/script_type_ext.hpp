#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class _ScriptTypeExt {
public:
	std::string name;
	std::vector<ScriptVar> vars;
};

CE_END_ED_NAMESPACE