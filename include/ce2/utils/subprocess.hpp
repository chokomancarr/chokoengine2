#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Subprocess { CE_CLASS_COMMON
    typedef std::function<void(const std::string&)> _CbFunc;
    
public:
    static void Run(const std::string& program, const std::vector<std::string>& args, _CbFunc callback = 0);
};

CE_END_NAMESPACE