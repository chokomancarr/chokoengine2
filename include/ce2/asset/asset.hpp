#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Asset : public _Object { CE_OBJECT_COMMON
protected:
    _Asset() = default;

public:
    virtual ~_Asset() = default;
};

CE_END_NAMESPACE

#include "./animclip.hpp"
#include "./animgraph.hpp"
#include "./armature.hpp"
#include "./font.hpp"
#include "./mesh.hpp"
#include "./shader.hpp"
#include "./material.hpp"
#include "./texture.hpp"
#include "./background.hpp"
#include "./cubemap.hpp"
#include "./render_target.hpp"