#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_IS_Component : public EW_I_Status {
public:
    float height = 2;

    bool expanded = true;
};

CE_END_ED_NAMESPACE