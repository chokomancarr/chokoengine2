#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Asset : public Object { CE_OBJECT_COMMON
protected:
    _Asset() = default;

public:
    virtual ~_Asset() = default;
};

CE_END_NAMESPACE

#include "asset/font.hpp"
#include "asset/shader.hpp"
#include "asset/texture.hpp"
#include "asset/render_target.hpp"