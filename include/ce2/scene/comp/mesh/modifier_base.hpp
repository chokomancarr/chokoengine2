#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _MeshModifier : public _Object { CE_OBJECT_COMMON
	VertexArray result;

    virtual void Apply(const VertexArray& vao_in, const VertexBuffer& elo_in) = 0;

public:
    virtual ~_MeshModifier() = default;
};

CE_END_NAMESPACE