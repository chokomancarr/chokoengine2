#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Material : public _Asset { CE_OBJECT_COMMON

    Shader _shader;

    
public:
    _Material();

    CE_GET_MEMBER(shader);
    CE_SET_MEMBER_F(shader);
};

CE_END_NAMESPACE