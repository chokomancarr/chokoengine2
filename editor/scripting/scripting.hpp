#pragma once
#include "chokoeditor.hpp"
#include "script_info.hpp"

CE_BEGIN_ED_NAMESPACE

class Scripting {
public:
	static ScriptInfo ParseInfo(const std::string& sig);
};

CE_END_ED_NAMESPACE