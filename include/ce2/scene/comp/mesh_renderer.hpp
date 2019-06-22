#pragma once
#include "chokoengine.hpp"
#include "mesh/modifier_base.hpp"

CE_BEGIN_NAMESPACE

class _MeshRenderer : _Component { CE_COMPONENT_COMMON
    VertexBuffer _posVbo, _nrmVbo, _texVbo;
    VertexObject _vao;
    VertexBuffer _elo;

    Mesh _mesh;
    std::vector<MeshModifier> _modifiers;
    std::vector<Material> _materials;
public:
    CE_GET_MEMBER(mesh);
    CE_SET_MEMBER_F(mesh);
    /* The list of modifiers applied to this mesh
     */
    CE_GET_MEMBER(modifiers);

    CE_GET_MEMBER(materials);
    CE_SET_MEMBER_F(materials);


    /* Adds a modifier at location \p index
     */
    void AddModifier(const MeshModifier& mod, int index = -1);
};

CE_END_NAMESPACE

#include "mesh/modifier_skin.hpp"