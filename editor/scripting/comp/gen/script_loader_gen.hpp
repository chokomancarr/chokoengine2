/*
 * Generate this
 * Example given:
 *   foo.hpp
 *   bar.hpp
 */

#pragma once
#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

class _Foo : public _Script {
public:
    CE_SERIALIZE int i;
    CE_SERIALIZE float f;
    CE_SERIALIZE Texture tx;
};
class _Bar : public _Script {};

class _ScrLdImpl_Foo;
class _ScrLdImpl_Bar;

typedef std::shared_ptr<_ScriptLoaderImpl> ScriptLoaderImpl;

class _ScriptLoaderImpl : public _ScriptLoaderBase {
protected:
    const std::string _sig;
    const std::vector<ScriptVarEntry> _entries;

    Component tar;

    std::unordered_map<std::string, ScriptLoaderImpl> _lds;

    ScriptLoaderImpl getld(const std::string& s) {
        auto& res = _lds[s];
        if (!res) { //generate this block
            if (s == "foo") {
                res = std::make_shared<_ScrLdImpl_Foo>();
            }
            else if (s == "bar") {
                res = std::make_shared<_ScrLdImpl_Bar>();
            }
        }
        return res;
    }

    _ScriptLoaderImpl(const std::string& s, const std::vector<ScriptVarEntry>& e)
        : _sig(s), _entries(e) {}

public:
    static void Init() {
        instance = std::make_shared<_ScriptLoaderImpl>();
    }
    CE_PL_NS ScriptLoaderBase GetLoaderOf(const std::string& s) override {
        return getld(s);
    }
    std::string sig() override {
        return _sig;
    }
    void activeTarget(const Component& t) override {
        tar = t;
    }

    std::vector<ScriptVarEntry> GetVarsOf(const std::string& s) override {
        return getld(s)->_entries;
    }
};

#define addentry(nm, tp) ScriptVarEntry(#nm, ScriptVarEntry::Type::tp)
#define addentrys(nm, tp, stp) ScriptVarEntry(#nm, ScriptVarEntry::Type::tp, #stp)

//generate these loaders
class _ScrLdImpl_Foo : public _ScriptLoaderImpl {
    _ScrLdImpl_Foo() : _ScriptLoaderImpl("foo", {
        addentry(i, Int),
        addentry(f, Float),
        addentrys(tx, Asset, Texture)
    }) {}

    Component Instantiate() override {
        return Ref<_Foo>::New();
    }

    void set_int(const std::string& s, const int& vl) override {
        auto scr = Ref<_Foo>(tar);
        if (s == "i") scr->i = vl;
    }
    int get_int(const std::string& s) override {
        auto scr = Ref<_Foo>(tar);
        if (s == "i") return scr->i;
    }
    //.... similar ....
    void set_Asset(const std::string& s, const Asset& a) override {
        auto scr = Ref<_Foo>(tar);
        if (s == "tx") scr->tx = (Texture)a;
    }
    Asset get_Asset(const std::string& s) override {
        auto scr = Ref<_Foo>(tar);
        if (s == "tx") return scr->tx;
    }
};
class _ScrLdImpl_Bar : public _ScriptLoaderImpl {
    //.... similar ....
};

CE_END_PL_NAMESPACE