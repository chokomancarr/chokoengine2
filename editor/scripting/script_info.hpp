#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class _ScriptInfo : public _Object {
public:
	std::string className;

	std::vector<ScriptTypeExt> types;
	std::vector<ScriptVar> vars;
};

CE_END_ED_NAMESPACE