#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ECallbackArg {
public:
    ECallbackArg(const std::string&, int);
    ECallbackArg(const std::string&, float);
    ECallbackArg(const std::string&, const std::string&);
    ECallbackArg(const std::string&, const pObject&);

    std::string name;
    int i;
    float f;
    std::string s;
    pObject o;
};

class ECallbackArgs {
    std::vector<ECallbackArg> _args;

public:
    ECallbackArgs(const std::vector<ECallbackArg>& args = {});

    void Add(const ECallbackArg&);

    const ECallbackArg& operator [](int) const;
};

CE_END_ED_NAMESPACE