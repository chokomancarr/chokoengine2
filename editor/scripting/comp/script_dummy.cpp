#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

_DummyScript::_DummyScript(const ScriptInfo& i) {
	info(i);
}

void _DummyScript::info(const pScriptInfo& i) {
	_info = i;
	if (!i) {
		name("Script (None)");
		return;
	}
	name(i->className + " (Script)");

	//
	vals.clear();
	for (auto& v : i->vars) {
		vals.push_back(v);
	}
}

CE_END_ED_NAMESPACE