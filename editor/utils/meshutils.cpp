#include "chokoeditor.hpp"
#include "meshutils.hpp"
#include "glutils.hpp"
#include "glsl/minVert.h"
#include "glsl/uvinfo.h"
#include "glsl/uvjmpgen.h"

CE_BEGIN_ED_NAMESPACE

MeshSurfaceData MeshUtils::GenSurfaceData(const Mesh& m) {
	auto approx3 = [](const Vec3& v1, const Vec3& v2) {
		return std::abs(v1.x - v2.x) < 0.000001f
			&& std::abs(v1.y - v2.y) < 0.000001f
			&& std::abs(v1.z - v2.z) < 0.000001f;
	};

	MeshSurfaceData data = {};

	const auto& poss = m->positions();
	const auto& uvs = m->texcoords();
	const auto& inds = m->triangles();

	data.vertCount = m->vertexCount();
	data.indCount = m->triangleCount();

	//--------- mesh buffers -----------
	data.positions = TextureBuffer::New(
		VertexBuffer_New(true, 3, data.vertCount, m->positions().data()),
		GL_RGB32F);
	data.uvcoords = TextureBuffer::New(
		VertexBuffer_New(true, 2, data.vertCount, m->texcoords().data()),
		GL_RG32F);
	data.indices = TextureBuffer::New(
		VertexBuffer_New(false, 3, data.vertCount, m->triangles().data()),
		GL_RGB32I);

	//unique vert indices
	int uvcnt = 1;
	std::vector<int> i2v(1, 0); //index -> unique index

	for (int a = 1; a < data.vertCount; a++) {
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
	edata.reserve(data.indCount * 3);
	for (auto& t : m->triangles()) {
		edata.push_back(glm::vec4(
			uvs[t.x] - uvs[t.y], uvs[t.z] - uvs[t.x]
		));
		edata.push_back(Vec4(poss[t.y] - poss[t.x], 0));
		edata.push_back(Vec4(poss[t.z] - poss[t.x], 0));
	}

	data.edgeData = TextureBuffer::New(
		VertexBuffer_New(true, 4, data.indCount * 3, edata.data()),
		GL_RGBA32F);

	//--------- triangle links -----------

	//indices to connected triangles
	std::vector<Int3> icons(data.indCount * 3, Int3(-1));

	//unmatched edges
	std::unordered_map<uint64_t, Int4> tvst = {}; //v1, v2, ve, t

	//vertex ids of edge
	std::vector<Int2> tvs = {};

	//adj graph of unique verts
	for (int a = 0; a < data.indCount; a++) {
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

#define COMB(a, b) (((uint64_t)a) << 32 | b)
		for (int b = 0; b < 3; b++) {
			const int k = a * 3 + b;
			const auto vt1 = vts[es[b][0]];
			const auto vt2 = vts[es[b][1]];
			if (tvst.count(COMB(vt1, vt2)) == 1) {
				const auto& res = tvst[COMB(vt1, vt2)];
				icons[k] = Int3(res);
				icons[res.w] = Int3(ts[es[b][0]], ts[es[b][1]], ts[es[b][2]]);
			}
			else if (tvst.count(COMB(vt2, vt1)) == 0) {
				const auto& res = tvst[COMB(vt1, vt2)];
				icons[k] = Int3(res);
				icons[res.w] = Int3(ts[es[b][0]], ts[es[b][1]], ts[es[b][2]]);
			}
			else {
				tvst[COMB(vt1, vt2)] = Int4(ts[es[b][0]], ts[es[b][1]], ts[es[b][2]], k);
			}
		}
	}

	data.iconData = TextureBuffer::New(
		VertexBuffer_New(false, 3, data.indCount, icons.data()),
		GL_RGB32I);

	//--------- uv angles -----------

	std::vector<Vec4> cons(data.indCount * 6, Vec4(-1));
	for (int a = 0; a < data.indCount; a++) {
		for (int b = 0; b < 3; b++) {
			const int k = a * 3 + b;
			const auto& ic = icons[k];
			if (ic.x > -1) {
				cons[k*2] = glm::vec4(uvs[ic.x], uvs[ic.y]);
				const auto tv = glm::normalize(uvs[inds[a][(b == 2) ? 0 : b + 1]] - uvs[inds[a][b]]);
				float t = std::acos(tv.x) * Math::rad2deg;
				if (tv.y > 0) t = 360 - t;
				t -= 90;
				cons[a * 2 + 1].x = t;
			}
		}
	}

	data.conData = TextureBuffer::New(
		VertexBuffer_New(true, 4, data.indCount * 6, cons.data()),
		GL_RGBA32F);

	return data;
}

void MeshUtils::SurfaceBlur(const MeshSurfaceData& data, const RenderTarget& tar, float size) {

}


bool MeshSurfaceData::initd = false;
Shader MeshSurfaceData::uvInfoShad;
Shader MeshSurfaceData::jmpInfoShad;

void MeshSurfaceData::InitShaders() {
	(uvInfoShad = Shader::New(glsl::uvinfoVert, glsl::uvinfoFrag))
		->AddUniforms({ "uvTex", "elemTex" });
	(jmpInfoShad = Shader::New(glsl::minVert, glsl::uvjmpgenFrag))
		->AddUniforms({ "uvinfo", "edgeData", "reso" });
	initd = true;
}

void MeshSurfaceData::GenInfoTex(const Int2& res) {
	if (!initd) InitShaders();

	glViewport(0, 0, res.x, res.y);

	uvInfoTex = FrameBuffer_New(res.x, res.y, { GL_RGBA32I });
	uvInfoTex->Bind();
	uvInfoTex->Clear();

	uvInfoShad->Bind();

	glUniform1i(uvInfoShad->Loc(0), 0);
	glActiveTexture(GL_TEXTURE0);
	uvcoords->Bind();
	glUniform1i(uvInfoShad->Loc(1), 1);
	glActiveTexture(GL_TEXTURE1);
	indices->Bind();
	GLUtils::DrawArrays(GL_TRIANGLES, indCount * 3);

	uvInfoShad->Unbind();
	uvInfoTex->Unbind();

	jmpInfoTex = FrameBuffer_New(res.x, res.y, { GL_RGBA32F });
	jmpInfoTex->Bind();
	jmpInfoTex->Clear();

	jmpInfoShad->Bind();

	glUniform1i(jmpInfoShad->Loc(0), 0);
	glActiveTexture(GL_TEXTURE0);
	uvInfoTex->tex(0)->Bind();
	glUniform1i(jmpInfoShad->Loc(1), 1);
	glActiveTexture(GL_TEXTURE1);
	conData->Bind();
	glUniform2f(jmpInfoShad->Loc(2), res.x, res.y);
	GLUtils::DrawArrays(GL_TRIANGLES, 6);

	jmpInfoShad->Unbind();
	jmpInfoTex->Unbind();
}

CE_END_ED_NAMESPACE