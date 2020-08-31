#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ScriptVal ScriptVal::invalid = ScriptVal();

ScriptVal::ScriptVal(const ScriptVar& vr, const ScriptInfo& info)
		: var(vr), _data() {
	if (vr.type == ScriptVar::Type::Class) {
		for (auto& sc : info->classes) {
			if (sc.name == vr.sub_class) {
				val_class.var = sc;
				val_class.val.resize(sc.vars.size());
			}
		}
	}
}

CE_END_ED_NAMESPACE