#pragma once
#include "chokoeditor.hpp"
#include "chokoplayer.hpp"

CE_BEGIN_ED_NAMESPACE

CE_OBJECT(ScriptInfo);
typedef CE_PL_NS ScriptVarEntry ScriptVar;
class ScriptVal;
CE_OBJECT(DummyScript)

class EScripting {
public:
	static void Init();

	static ScriptInfo ParseInfo(const std::string& sig);
};

CE_END_ED_NAMESPACE

#include "script_info.hpp"
#include "script_val.hpp"
#include "comp/script_dummy_loader.hpp"
#include "loader_generator.hpp"