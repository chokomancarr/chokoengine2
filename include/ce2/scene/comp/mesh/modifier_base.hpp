#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _MeshModifier { CE_OBJECT_COMMON
    
    VertexBuffer _posVboI, _nrmVboI, _texVboI;
    VertexObject _vao;
    VertexBuffer _elo;
    
    /* if a buffer is null, the original is propogated
     */
    VertexBuffer _posVboO, _nrmVboO, _texVboO;

    virtual void Apply(
        const VertexBuffer&,
        const VertexBuffer&,
        const VertexBuffer&
    ) = 0;
public:
    _MeshModifier();
    virtual ~_MeshModifier() = default;

    
};

CE_END_NAMESPACE