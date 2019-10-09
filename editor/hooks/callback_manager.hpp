#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ECallbackManager {
public:
    static void Register();
    static void Invoke(const std::string& sig, const ECallbackArgs args);
};

CE_END_ED_NAMESPACE