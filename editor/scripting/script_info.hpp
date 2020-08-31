#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class _ScriptInfo : public _Object {
public:
	std::string className;
	std::string sig;

	std::vector<CE_PL_NS ScriptClassEntry> classes;
	std::vector<CE_PL_NS ScriptVarEntry> vars;
};

CE_END_ED_NAMESPACE