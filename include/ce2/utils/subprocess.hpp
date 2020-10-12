#pragma once
#include "chokoengine.hpp"
#include <functional>

CE_BEGIN_NAMESPACE

class Subprocess { CE_CLASS_COMMON
    typedef std::function<void(const std::string&)> _CbFunc;
    
public:
    struct ProcessInfo {
        std::string program;
        std::string workingDir;
		std::string setenvCmd; //this is a windows thing, should remove
        std::vector<std::string> args;
        bool wait;
        _CbFunc callback;

        ProcessInfo() : program(""), workingDir(""), setenvCmd(""),
            args({}), wait(true), callback(nullptr) {}
    };

    static int Run(const std::string& program, const std::vector<std::string>& args, _CbFunc callback = 0);
    
    static int Run(const ProcessInfo& info);
};

CE_END_NAMESPACE