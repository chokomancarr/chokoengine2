#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _MeshModifier : public _Object { CE_OBJECT_COMMON
	VertexObject result;

    virtual void Apply(const VertexObject& vao_in, const VertexBuffer& elo_in) = 0;

public:
    virtual ~_MeshModifier() = default;
};

CE_END_NAMESPACE