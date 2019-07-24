#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_IS_SceneObject : public EW_I_Status {
public:
    EW_I_Status::UMap components = EW_I_Status::UMap();
};

CE_END_ED_NAMESPACE