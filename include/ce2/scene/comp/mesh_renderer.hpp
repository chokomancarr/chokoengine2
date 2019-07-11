#pragma once
#include "chokoengine.hpp"
#include "mesh/modifier_base.hpp"

CE_BEGIN_NAMESPACE

class _MeshRenderer : public _Component { CE_COMPONENT_COMMON
    Mesh _mesh;
    std::vector<MeshModifier> _modifiers;
    std::vector<Material> _materials;

public:
	_MeshRenderer();

	/* The mesh data
	*/
    CE_GET_SET_MEMBER_F(mesh);

    /* The list of modifiers applied to this mesh
     */
    CE_GET_MEMBER(modifiers);

	/* The list of materials
	 * Some materials can be null
	 */
    CE_GET_SET_MEMBER_F(materials);

    /* Adds a modifier at location \p index
     */
	template <typename T>
	void AddModifier(int index = -1);

	void OnUpdate() override;
};

CE_END_NAMESPACE

#include "mesh/modifier_skin.hpp"
#include "detail/mesh_renderer.inl"