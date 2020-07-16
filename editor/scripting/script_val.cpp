#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ScriptVal ScriptVal::invalid = ScriptVal(ScriptVar());

ScriptVal::ScriptVal(const ScriptVar& vr)
		: var(vr), _data() {}

CE_END_ED_NAMESPACE