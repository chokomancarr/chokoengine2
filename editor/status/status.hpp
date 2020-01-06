#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EStatus {
public:
    static void Reg(const std::string&);
    /* set progress value (0 ~ 1)
     */
    static void Set(const std::string&, float);
    static void Unreg(const std::string&);

    static bool busy();

    static void DrawHeader();
};

CE_END_ED_NAMESPACE
