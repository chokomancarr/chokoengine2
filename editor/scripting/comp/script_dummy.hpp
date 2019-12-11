#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class _DummyScript : public _Script {
	pScriptInfo _info;
public:
	_DummyScript(const ScriptInfo& = nullptr);

	CE_GET_SET_MEMBER_F(info);

	std::vector<ScriptVal> vals;
};

CE_END_ED_NAMESPACE