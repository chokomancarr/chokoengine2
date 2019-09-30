#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EPaths {
    static std::unordered_map<std::string, std::string> paths;
public:
    static void Init();

    static const std::string& Get(const std::string& sig, const std::string& def = "");
};

CE_END_ED_NAMESPACE