#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void _DummyScript::info(const ScriptInfo& i) {
	_info = i;
	if (!i) return;
	//
	vals.clear();
	for (auto& v : i->vars) {
		vals.push_back(v);
	}
}

CE_END_ED_NAMESPACE