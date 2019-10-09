#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

/* Argument for a callback
 */
class ECallbackArg {
public:
    ECallbackArg(const std::string&, int);
    ECallbackArg(const std::string&, float);
    ECallbackArg(const std::string&, const std::string&);
    ECallbackArg(const std::string&, const pObject&);

    std::string name; /* name of the argument */

    /* values
     */
    int i;
    float f;
    std::string s;
    pObject o;
};

/* Argument list for a callback
 */
class ECallbackArgs {
    std::vector<ECallbackArg> _args;

public:
    ECallbackArgs(const std::vector<ECallbackArg>& args = {});

    void Add(const ECallbackArg&);

    CE_GET_MEMBER(args);

    const ECallbackArg& operator [](int) const;
};

CE_END_ED_NAMESPACE