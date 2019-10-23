#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Script : public _Component { CE_COMPONENT_COMMON
protected:
	_Script();
    
public:
    _Script(const _Script&) = default;

    virtual ~_Script() = default;
};

CE_END_NAMESPACE