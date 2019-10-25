#include "chokoeditor.hpp"
#include "meshutils.hpp"

CE_BEGIN_ED_NAMESPACE

MeshSurfaceData MeshUtils::GenSurfaceData(const Mesh& m, const Int2& res) {
	auto approx3 = [](const Vec3& v1, const Vec3& v2) {
		return std::abs(v1.x - v2.x) < 0.000001f
			&& std::abs(v1.y - v2.y) < 0.000001f
			&& std::abs(v1.z - v2.z) < 0.000001f;
	};

	MeshSurfaceData data;

	const auto& poss = m->positions();
	const auto& uvs = m->texcoords();
	const auto& inds = m->triangles();

	//--------- mesh buffers -----------
	data.positions = TextureBuffer::New(
		VertexBuffer_New(true, 3, m->vertexCount(), m->positions().data()),
		GL_RGB32F);
	data.uvcoords = TextureBuffer::New(
		VertexBuffer_New(true, 2, m->vertexCount(), m->texcoords().data()),
		GL_RGB32F);
	data.indices = TextureBuffer::New(
		VertexBuffer_New(false, 3, m->vertexCount(), m->triangles().data()),
		GL_RGB32I);

	//unique vert indices
	int uvcnt = 1;
	std::vector<int> i2v(1, 0); //index -> unique index

	for (int a = 1; a < m->vertexCount(); a++) {
		const auto& va = poss[a];
		for (int b = 0; b < uvcnt; b++) {
			if (approx3(poss[i2v[b]], va))
				goto brk;
		}
		i2v.push_back(a);
		uvcnt++;
	brk:;
	}
	
	//--------- edge vectors -----------
	std::vector<Vec4> edata = {};
	edata.reserve(m->triangleCount() * 3);
	for (auto& t : m->triangles()) {
		edata.push_back(glm::vec4(
			uvs[t.x] - uvs[t.y], uvs[t.z] - uvs[t.x]
		));
		edata.push_back(Vec4(poss[t.y] - poss[t.x], 0));
		edata.push_back(Vec4(poss[t.z] - poss[t.x], 0));
	}

	data.edgeData = TextureBuffer::New(
		VertexBuffer_New(true, 4, m->triangleCount() * 3, edata.data()),
		GL_RGBA32F);

	//--------- triangle links -----------

	//indices to connected triangles
	std::vector<Int3> icons(m->triangleCount() * 3, Int3(-1));

	//unmatched edges
	std::unordered_map<uint64_t, Int4> tvst = {}; //v1, v2, ve, t

	//vertex ids of edge
	std::vector<Int2> tvs = {};

	//adj graph of unique verts
	for (int a = 0; a < m->triangleCount(); a++) {
		const auto& ts = inds[a];
		const int vts[] = {
			i2v[ts[0]],
			i2v[ts[1]],
			i2v[ts[2]]
		};

		const int es[3][3] = {
			{ 0, 1, 2 },
			{ 1, 2, 0 },
			{ 2, 0, 1 }
		};

		for (int b = 0; b < 3; b++) {
			const int k = a * 3 + b;
			const auto vt1 = vts[es[b][0]];
			const auto vt2 = vts[es[b][1]];
			if (tvst.count((vt1 << 32) | vt2) == 1) {
				const auto& res = tvst[(vt1 << 32) + vt2];
				icons[k] = Int3(res);
				icons[res.w] = Int3(ts[es[b][0]], ts[es[b][1]], ts[es[b][2]]);
			}
			else if (tvst.count((vt2 << 32) | vt1) == 0) {
				const auto& res = tvst[(vt1 << 32) + vt2];
				icons[k] = Int3(res);
				icons[res.w] = Int3(ts[es[b][0]], ts[es[b][1]], ts[es[b][2]]);
			}
			else {
				tvst[vt1 << 32 | vt2] = Int4(ts[es[b][0]], ts[es[b][1]], ts[es[b][2]], k);
			}
		}
	}

	data.iconData = TextureBuffer::New(
		VertexBuffer_New(false, 3, m->triangleCount(), icons.data()),
		GL_RGB32I);

	//--------- uv angles -----------

	std::vector<Vec4> cons(inds.size() * 2, Vec4(-1));
	for (int a = 0; a < m->triangleCount(); a++) {
		if (icons[a * 3] > -1) {
			cons[a*2] = Vec4(uvs[icons[a * 3]], uvs[icons[a * 3 + 1]]);
			Vec2 tv = Normalize(uvs[inds[(a % 3 == 2) ? a - 2 : a + 1]] - uvs[inds[a]]);
			float t = std::acos(tv.x) * rad2deg;
			if (tv.y > 0) t = 360 - t;
			t -= 90;
			cons[a * 2 + 1].x = t;
		}
	}
}

CE_END_ED_NAMESPACE