#pragma once
#include "chokoeditor.hpp"
#include "script_dummy.hpp"

CE_BEGIN_ED_NAMESPACE

class _DummyScriptLoader : public CE_PL_NS _ScriptLoaderBase {
    ScriptInfo _info;

public:
    static void Init();

    CE_PL_NS ScriptLoaderBase GetLoaderOf(const std::string&) override;

    std::string sig() override;

    Component Instantiate() override;

    void activeTarget(const Component&) override;

    std::vector<ScriptVar> GetVarsOf(const std::string&) override;

    CE_PL_SCR_GETSET_DEFS
};

#undef CE_PL_SCR_GETSET

CE_END_PL_NAMESPACE