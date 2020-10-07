#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

void _Mesh::GenBuffers() {
	const auto vsz = vertexCount();
	_posVbo = VertexBuffer_New(true, 3, vsz, _positions.data());
	_nrmVbo = VertexBuffer_New(true, 3, vsz, _normals.data());
	_tanVbo = VertexBuffer_New(true, 3, vsz, _tangents.data());
	_texVbo = VertexBuffer_New(true, 2, vsz, _texcoords.data());
	_vao = VertexArray_New();
	_vao->AddBuffer(_posVbo);
	_vao->AddBuffer(_nrmVbo);
	_vao->AddBuffer(_tanVbo);
	_vao->AddBuffer(_texVbo);

	const auto msz = materialCount();
	_elos.resize(msz);
	for (size_t a = 0; a < msz; a++) {
		_elos[a] = VertexBuffer_New(false, 3, _matTriangles[a].size(), _matTriangles[a].data(), 0, GL_ELEMENT_ARRAY_BUFFER);
	}
}

_Mesh::_Mesh() : _Asset(AssetType::Mesh), _positions({}), _normals({}), _tangents({}), _texcoords({}),
		_triangles({}), _matTriangles({}), _vertexGroups({}), _shapeKeys({}) {}

size_t _Mesh::vertexCount() const {
	return _positions.size();
}

size_t _Mesh::triangleCount() const {
	return _triangles.size();
}

size_t _Mesh::materialCount() const {
	return _matTriangles.size();
}

void _Mesh::CalculateTangents() {
	const auto vsz = vertexCount();
	_tangents.clear();
	_tangents.resize(vsz, Vec3(0));
	std::vector<bool> tC(vsz, 0);

	if (!_texcoords.size()) {
		Debug::Warning("Mesh::GenerateTangents", "Mesh does not have UV coordinates!");
		return;
	}

	Vec2 u0, u1, u2, r1, r2;
	Vec3 p0, p1, p2;
	float a, b, db;
	int n0, n1, n2;
	const auto tsz = triangleCount();
	for (size_t n = 0; n < tsz; n++) {
		n0 = _triangles[n].x;
		n1 = _triangles[n].y;
		n2 = _triangles[n].z;

		u0 = _texcoords[n0];
		u1 = _texcoords[n1];
		u2 = _texcoords[n2];
		p0 = _positions[n0];
		p1 = _positions[n1];
		p2 = _positions[n2];

		if ((u0 == u1) || (u1 == u2) || (u0 == u2)) {
			//Debug::Warning("Tangent calculator", "Triangle " + std::to_string(n) + " does not have well-defined UVs!");
			continue;
		}
		r1 = u1 - u0;
		r2 = u2 - u0;

		db = r1.y*r2.x - r1.x*r2.y;
		if (db == 0) {
			//Debug::Warning("Tangent calculator", "Triangle " + std::to_string(n) + " does not have well-defined UVs!");
			continue;
		}
		b = r1.y / db;
		if (r1.y == 0) a = (1 - b * r2.x) / r1.x;
		else a = -b * r2.y / r1.y;
		Vec3 t = (p1 - p0)*a + (p2 - p0)*b;
		_tangents[n0] += t;
		_tangents[n1] += t;
		_tangents[n2] += t;
		tC[n0] = tC[n1] = tC[n2] = true;
	}
	for (uint a = 0; a < vsz; a++) {
		if (tC[a]) _tangents[a] = _tangents[a].normalized();
	}
}

void _Mesh::Apply() {
	GenBuffers();
}

void _Mesh::BindVao() const {
	_vao->Bind();
}

void _Mesh::BindElo(int matid) const {
	_elos[matid]->Bind();
}

void _Mesh::Unbind() const {
	_vao->Unbind();
	_elos[0]->Unbind();
}

CE_END_NAMESPACE