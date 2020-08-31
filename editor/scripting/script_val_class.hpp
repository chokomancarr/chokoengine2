#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ScriptValClass {
public:
	ScriptClass var;

	std::vector<ScriptVal> val;

	static ScriptValClass invalid;
};

CE_END_ED_NAMESPACE