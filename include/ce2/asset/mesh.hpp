#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Mesh : public _Asset { CE_OBJECT_COMMON
public:
	struct VertexGroup {
		std::string name;
		std::vector<float> weights;

		VertexGroup(size_t n) : name(""), weights(n) {}
	};
	struct ShapeKey {
		std::string name;
		std::vector<Vec3> offsets;
	};

private:
	std::vector<Vec3> _positions;
	std::vector<Vec3> _normals;
	std::vector<Vec3> _tangents; 
	std::vector<Vec2> _texcoords;

	std::vector<Int3> _triangles;
	std::vector<std::vector<Int3>> _matTriangles;

	std::vector<VertexGroup> _vertexGroups;

	std::vector<ShapeKey> _shapeKeys;

	VertexBuffer _posVbo, _nrmVbo, _tanVbo, _texVbo;
	VertexArray _vao;
	std::vector<VertexBuffer> _elos;

	void GenBuffers();
public:
	_Mesh();

	size_t vertexCount() const;
	size_t triangleCount() const;
	size_t materialCount() const;
	CE_GET_SET_MEMBER(positions);
	CE_GET_SET_MEMBER(normals);
	CE_GET_SET_MEMBER(tangents);
	CE_GET_SET_MEMBER(texcoords);
	CE_GET_SET_MEMBER(triangles);
	CE_GET_SET_MEMBER(matTriangles);
	CE_GET_SET_MEMBER(vertexGroups);
	CE_GET_SET_MEMBER(shapeKeys);

	/* Generate tangent data
	 * UV coordinates must be well-defined for all vertices
	 */
	void CalculateTangents();

	/* Apply must be called after setting vertex data
	 */
	void Apply();

	friend class _MeshRenderer;
};

CE_END_NAMESPACE
