#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_OBJECT(ScriptInfo);
class ScriptVar;
class _ScriptTypeExt;
typedef std::shared_ptr<_ScriptTypeExt> ScriptTypeExt;
typedef std::weak_ptr<_ScriptTypeExt> pScriptTypeExt;
CE_OBJECT(DummyScript)

class Scripting {
public:
	static ScriptInfo ParseInfo(const std::string& sig);
};

CE_END_ED_NAMESPACE

#include "script_info.hpp"
#include "script_var.hpp"
#include "script_type_ext.hpp"
#include "comp/script_dummy.hpp"