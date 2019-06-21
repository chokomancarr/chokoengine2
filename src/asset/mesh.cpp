#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_Mesh::_Mesh() : _positions({}), _normals({}), _tangents({}), _texcoords({}),
		_triangles({}), _matTriangles({}), _vertexGroups({}), _shapeKeys({}) {}

CE_END_NAMESPACE