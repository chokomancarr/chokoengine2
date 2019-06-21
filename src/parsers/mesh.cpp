#include "chokoengine.hpp"
#include "parsers/mesh.hpp"

CE_BEGIN_NAMESPACE

Mesh MeshLoader::LoadObj(const std::string& path) {
	std::ifstream strm(path, std::ios::binary);
	std::string s;
	if (!strm) return Mesh();

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
			_norms.push_back(glm::normalize(Vec3(
				std::stof(ss[1]),
				std::stof(ss[2]),
				std::stof(ss[3])
			)));
		}
		else if (ss0 == "vt") {
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
					if (ss1.size() > 1) {
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
						norms.push_back(_norms[i3.y - 1]);
						if (i3.z > -1) {
							uvs.push_back(_uvs[i3.z - 1]);
						}
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

	if (!verts.size() || !tris.size()) return Mesh();
	Mesh m = Mesh::New();
	m->positions(verts);
	m->normals(norms);
	m->texcoords(uvs);
	m->triangles(tris);
	m->matTriangles(mtris);
	return m;
}

CE_END_NAMESPACE