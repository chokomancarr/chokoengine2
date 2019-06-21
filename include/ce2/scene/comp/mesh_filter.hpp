#pragma once
#include "chokoengine.hpp"
#include "mesh/modifier_base.hpp"

CE_BEGIN_NAMESPACE

class _MeshFilter : _Component { CE_COMPONENT_COMMON
    VertexBuffer _posVbo, _nrmVbo, _texVbo;
    VertexObject _vao;
    VertexBuffer _elo;

    std::vector<MeshModifier> _modifiers;
public:
    /* The list of modifiers applied to this mesh
     */
    CE_GET_MEMBER(modifiers);

    /* Adds a modifier at location \p index
     */
    void AddModifier(const MeshModifier& mod, int index = -1);
};

CE_END_NAMESPACE

#include "mesh/modifier_skin.hpp"