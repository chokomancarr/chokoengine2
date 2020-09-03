#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ScriptVal ScriptVal::invalid = ScriptVal();

ScriptVal::ScriptVal(const ScriptVar& vr, const ScriptInfo& info)
		: var(vr), _data() {
	if (vr.type == ScriptVar::Type::Class) {
		for (auto& sc : info->classes) {
			if (sc.name == vr.sub_class) {
				val_class.var = sc;
				val_class.val.reserve(sc.vars.size());
				for (auto& v : sc.vars) {
					val_class.val.push_back(ScriptVal(v, info));
				}
			}
		}
	}
}

CE_END_ED_NAMESPACE