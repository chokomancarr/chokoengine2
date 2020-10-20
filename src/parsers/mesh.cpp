#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Mesh MeshLoader::LoadObj(const std::string& data) {
	std::stringstream strm(data);

	std::string s;
	//pool
	std::vector<Vec3> _verts;
	std::vector<Vec3> _norms;
	std::vector<Vec2> _uvs;

	std::vector<Vec3> verts;
	std::vector<Vec3> norms;
	std::vector<Vec2> uvs;
	std::vector<Int3> tris;
	std::vector<std::vector<Int3>> mtris(1);
	int triCnt = 0; //total number of unique indices
	int matCnt = -1;
	bool hasuv = false;

	std::unordered_map<uint64_t, int> triMap = {};

	while (strm) {
		std::getline(strm, s);
		if (s[0] == '#') continue;

		const auto ss = StrExt::Split(s, ' ', true);
		const auto& ss0 = ss[0];
		if (ss0 == "v") {
			_verts.push_back(Vec3(
				std::stof(ss[1]),
				std::stof(ss[2]),
				std::stof(ss[3])
			));
		}
		else if (ss0 == "vn") {
			_norms.push_back(Vec3(
				std::stof(ss[1]),
				std::stof(ss[2]),
				std::stof(ss[3])
			).normalized());
		}
		else if (ss0 == "vt") {
			hasuv = true;
			_uvs.push_back(Vec2(
				std::stof(ss[1]),
				std::stof(ss[2])
			));
		}
		else if (ss0 == "usemtl") {
			matCnt++;
			if (matCnt > 0) {
				mtris.push_back({});
			}
		}
		else if (ss0 == "f") {
			Int3 t;
			for (int a = 0; a < 3; a++) {
				const auto ss1 = StrExt::Split(ss[a + 1], '/');
				auto i3 = Int3(std::stoi(ss1[0]), -1, -1);
				if (ss1.size() > 1) {
					if (ss1[1] != "")
						i3.y = std::stoi(ss1[1]);
					if (ss1.size() > 2 && ss1[2] != "") {
						i3.z = std::stoi(ss1[2]);
					}
				}
				const auto mk = ((uint64_t)i3.x << 40) + ((uint64_t)i3.y << 20) + (uint64_t)i3.z;
				const auto ti = triMap[mk];
				if (!ti) {
					t[a] = triCnt;
					triMap[mk] = triCnt;
					verts.push_back(_verts[i3.x - 1]);
					if (i3.y > -1) {
						uvs.push_back(_uvs[i3.y - 1]);
					}
					if (i3.z > -1) {
						norms.push_back(_norms[i3.z - 1]);
					}
					triCnt++;
				}
				else {
					t[a] = ti;
				}
			}
			tris.push_back(t);
			mtris.back().push_back(t);
		}
	}

	if (!verts.size()) {
		Debug::Warning("Mesh Parser", "vertex count is zero!");
		return nullptr;
	}
	if (!tris.size()) {
		Debug::Warning("Mesh Parser", "face count is zero!");
		return nullptr;
	}

	Mesh m = Mesh::New();
	m->positions(verts);
	m->normals(norms);
	m->texcoords(uvs);
	m->triangles(tris);
	m->matTriangles(mtris);
	m->CalculateTangents();
	m->Apply();
	return m;
}

#define READ(nm) strm.read((char*)&nm, sizeof(nm))

Mesh MeshLoader::LoadMesh(const std::string& path) {
	std::ifstream strm(path, std::ios::binary);
	return LoadMesh(strm);
}
Mesh MeshLoader::LoadMesh(std::istream& strm) {
	if (!strm) return nullptr;

	std::string s;
	std::vector<Vec3> verts;
	std::vector<Vec3> norms;
	std::vector<Vec2> uvs;
	std::vector<Int3> tris;
	std::vector<_Mesh::VertexGroup> grps;
	std::vector<_Mesh::ShapeKey> shps;
	std::vector<std::vector<Int3>> mtris(1);

	std::getline(strm, s, '\0');
	if (s != "ChokoEngine Mesh 20") {
		Debug::Error("MeshLoader::LoadMesh", "Invalid file signature!");
		return nullptr;
	}

	uint32_t vcnt = 0;
	uint8_t msz = 1;
	char c;
	while (READ(c)) {
		switch (c) {
		case 'V': {
			READ(vcnt);
			verts.reserve(vcnt);
			norms.reserve(vcnt);
			for (uint32_t a = 0; a < vcnt; a++) {
				Vec3 v, n;
				READ(v);
				READ(n);
				verts.push_back(v);
				norms.push_back(n);
			}
			break;
		}
		case 'F': {
			uint32_t fcnt;
			READ(fcnt);
			tris.reserve(fcnt);
			for (uint32_t a = 0; a < fcnt; a++) {
				uint8_t mid;
				READ(mid);
				if (msz <= mid) {
					msz = mid + 1;
					mtris.resize(msz);
				}
				Int3 tri;
				READ(tri);
				mtris[mid].push_back(tri);
				tris.push_back(tri);
			}
			break;
		}
		case 'U': {
			READ(c); //1
			uvs.reserve(vcnt);
			for (uint32_t a = 0; a < vcnt; a++) {
				Vec2 uv;
				READ(uv);
				uvs.push_back(uv);
			}
			break;
		}
		case 'G': {
			uint8_t ng = 0;
			READ(ng);
			grps.resize(ng, _Mesh::VertexGroup(vcnt));
			for (uint8_t g = 0; g < ng; g++) {
				std::getline(strm, grps[g].name, '\0');
			}
			uint8_t gi = 0;
			for (uint32_t a = 0; a < vcnt; a++) {
				READ(ng);
				for (uint8_t g = 0; g < ng; g++) {
					READ(gi);
					READ(grps[gi].weights[a]);
				}
			}
		}
		case 'S': {
			uint8_t ns = 0;
			READ(ns);
			shps.resize(ns);
			for (uint8_t s = 0; s < ns; s++) {
				auto& shp = shps[s];
				std::getline(strm, shp.name, '\0');
				shp.offsets.resize(vcnt);
				strm.read((char*)shp.offsets.data(), vcnt * sizeof(Vec3));
			}
		}
		default:
			goto asdf;
			break;
		}
	}
	asdf:

	if (!verts.size()) {
		Debug::Warning("Mesh Parser", "vertex count is zero!");
		return nullptr;
	}
	if (!tris.size()) {
		Debug::Warning("Mesh Parser", "face count is zero!");
		return nullptr;
	}

	Mesh m = Mesh::New();
	m->positions(verts);
	m->normals(norms);
	m->texcoords(uvs);
	m->triangles(tris);
	m->matTriangles(mtris);
	m->vertexGroups(grps);
	m->shapeKeys(shps);
	m->CalculateTangents();
	m->Apply();
	return m;
}

CE_END_NAMESPACE