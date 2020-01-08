#pragma once
#include "chokoplayer.hpp"
#include "script_var_entry.hpp"

/*
 * The program must implement the script loader derived from this class,
 * and set it to _ScriptLoaderBase::instance.
 */

CE_BEGIN_PL_NAMESPACE

#define CE_PL_SCR_GETSET(tp)\
    virtual void set_ ## tp(const std::string&, const tp&) = 0;\
    virtual tp get_ ## tp(const std::string&) = 0;

class _ScriptLoaderBase {
public:
    static CE_EXPORT ScriptLoaderBase instance;

    virtual ScriptLoaderBase GetLoaderOf(const std::string&) = 0;

    virtual std::string sig() = 0;

    virtual Component Instantiate() = 0;

    virtual void activeTarget(const Component&) = 0;

    virtual ~_ScriptLoaderBase() = default;

    virtual std::vector<ScriptVarEntry> GetVarsOf(const std::string&) = 0;

    CE_PL_SCR_GETSET(int)
    CE_PL_SCR_GETSET(float)
    CE_PL_SCR_GETSET(Vec2)
    CE_PL_SCR_GETSET(Vec3)
    CE_PL_SCR_GETSET(Vec4)
    CE_PL_SCR_GETSET(Quat)
    CE_PL_SCR_GETSET(Asset)
};

#undef CE_PL_SCR_GETSET

#define CE_PL_SCR_GETSET_DEF(tp)\
    void set_ ## tp(const std::string&, const tp&) override;\
    tp get_ ## tp(const std::string&) override;

#define CE_PL_SCR_GETSET_DEFS\
    CE_PL_SCR_GETSET_DEF(int)\
    CE_PL_SCR_GETSET_DEF(float)\
    CE_PL_SCR_GETSET_DEF(Vec2)\
    CE_PL_SCR_GETSET_DEF(Vec3)\
    CE_PL_SCR_GETSET_DEF(Vec4)\
    CE_PL_SCR_GETSET_DEF(Quat)\
    CE_PL_SCR_GETSET_DEF(Asset)

#define CE_PL_SCR_GETSET_EMPTY(tp)\
    void set_ ## tp(const std::string&, const tp&) override {}\
    tp get_ ## tp(const std::string&) override { CE_NOT_IMPLEMENTED }

CE_END_PL_NAMESPACE