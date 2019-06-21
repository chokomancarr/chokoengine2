#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Mesh : public _Asset { CE_OBJECT_COMMON
	std::vector<Vec3> _positions;
	std::vector<Vec3> _normals;
	std::vector<Vec3> _tangents; 
	std::vector<Vec2> _texcoords;

	std::vector<Int3> _triangles;
	std::vector<std::vector<Int3>> _matTriangles;

	struct VertexGroup {
		std::string name;
		std::vector<float> weights;
	};
	std::vector<VertexGroup> _vertexGroups;

	struct ShapeKey {
		std::string name;
		std::vector<Vec3> _offset;
	};
	std::vector<ShapeKey> _shapeKeys;

public:
	_Mesh();

	CE_GET_SET_MEMBER(positions);
	CE_GET_SET_MEMBER(normals);
	CE_GET_SET_MEMBER(tangents);
	CE_GET_SET_MEMBER(texcoords);
	CE_GET_SET_MEMBER(triangles);
	CE_GET_SET_MEMBER(matTriangles);
	CE_GET_SET_MEMBER(vertexGroups);
	CE_GET_SET_MEMBER(shapeKeys);
};

CE_END_NAMESPACE
