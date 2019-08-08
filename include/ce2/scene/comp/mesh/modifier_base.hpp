#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* A modifier applied to a mesh renderer
 */
class _MeshModifier : public _Object { CE_OBJECT_COMMON
protected:
	VertexArray result;

	pMeshRenderer parent;

	_MeshModifier(const std::string& nm) : _Object(nm) {}

	virtual void Apply(const VertexArray& vao_in) = 0;

	virtual void OnSetMesh(const Mesh& m) = 0;

public:
    virtual ~_MeshModifier() = default;

	friend class _MeshRenderer;
};

CE_END_NAMESPACE