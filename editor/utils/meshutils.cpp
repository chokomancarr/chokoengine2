#include "chokoeditor.hpp"
#include "meshutils.hpp"
#include "glutils.hpp"
#include "glsl/minVert.h"
#include "glsl/padTextureFrag.h"
#include "glsl/surfaceBlurFrag.h"
#include "glsl/uvinfo.h"
#include "glsl/uvinfo_exp.h"
#include "glsl/uvjmpgen.h"

CE_BEGIN_ED_NAMESPACE

bool MeshUtils::initd = false;

Shader MeshUtils::padShad;
Shader MeshUtils::blurShad;

void MeshUtils::Init() {
	(padShad = Shader::New(glsl::minVert, glsl::padTextureFrag))
		->AddUniforms({ "reso", "colTex", "jmpTex" });
	(blurShad = Shader::New(glsl::minVert, glsl::surfBlurFrag))
		->AddUniforms({
			"sres", "reso", "colTex", "idTex", 
			"jmpTex", "posBuf", "indBuf", "edatBuf",
			"iconBuf", "conBuf", "dir0", "blurcnt"
		});

	initd = true;
}

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
		for (int b = 0; b < a; b++) {
			if (approx3(poss[b], va)) {
				i2v.push_back(i2v[b]);
				goto brk;
			}
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
			uvs[t.y] - uvs[t.x], uvs[t.z] - uvs[t.x]
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
	for (int a = 0; a < data.indCount; a++) { //tri id
		const auto& ts = inds[a];
		const int vts[] = { //unique vert ids
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
		for (int b = 0; b < 3; b++) { //edge id
			const int k = a * 3 + b;
			const auto vt1 = vts[es[b][0]];
			const auto vt2 = vts[es[b][1]];
			if (tvst.count(COMB(vt1, vt2)) == 1) {
				const auto& res = tvst[COMB(vt1, vt2)];
				icons[k] = Int3(res);
				icons[res.w] = Int3(ts[es[b][1]], ts[es[b][0]], ts[es[b][2]]);
			}
			else if (tvst.count(COMB(vt2, vt1)) == 1) {
				const auto& res = tvst[COMB(vt2, vt1)];
				icons[k] = Int3(res);
				icons[res.w] = Int3(ts[es[b][1]], ts[es[b][0]], ts[es[b][2]]);
			}
			else {
				tvst[COMB(vt1, vt2)] = Int4(ts[es[b][1]], ts[es[b][0]], ts[es[b][2]], k);
			}
		}
	}

	data.iconData = TextureBuffer::New(
		VertexBuffer_New(false, 3, data.indCount * 3, icons.data()),
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

void MeshUtils::PadTexture(MeshSurfaceData& data, const Texture& src, const RenderTarget& tar) {
	if (!initd) Init();

	const auto w = src->width();
	const auto h = src->height();
	const auto info = data.GetInfoTex(Int2(w, h));

	glViewport(0, 0, w, h);

	padShad->Bind();

	glUniform2f(padShad->Loc(0), w, h);
	glUniform1i(padShad->Loc(1), 0);
	glActiveTexture(GL_TEXTURE0);
	src->Bind();
	glUniform1i(padShad->Loc(2), 1);
	glActiveTexture(GL_TEXTURE1);
	info.jmpInfoTex->tex(0)->Bind();

	tar->BindTarget();
	tar->Clear(Color(0, 0), 1);
	GLUtils::DrawArrays(GL_TRIANGLES, 6);

	tar->UnbindTarget();

	padShad->Unbind();
}

void MeshUtils::SurfaceBlur(MeshSurfaceData& data, const Texture& src,
		const RenderTarget& tar, const RenderTarget& tmp, float size) {
	if (!initd) Init();

	const auto w = src->width();
	const auto h = src->height();
	const auto info = data.GetInfoTex(Int2(w, h));

	glViewport(0, 0, w, h);

	blurShad->Bind();

	glUniform2f(blurShad->Loc(0), w, h);
	glUniform2f(blurShad->Loc(1), w, h);
	glUniform1i(blurShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	src->Bind();
	glUniform1i(blurShad->Loc(3), 1);
	glActiveTexture(GL_TEXTURE1);
	info.uvInfoTex->tex(0)->Bind();
	glUniform1i(blurShad->Loc(4), 2);
	glActiveTexture(GL_TEXTURE2);
	info.jmpInfoTex->tex(0)->Bind();
	glUniform1i(blurShad->Loc(5), 3);
	glActiveTexture(GL_TEXTURE3);
	data.positions->Bind();
	glUniform1i(blurShad->Loc(6), 4);
	glActiveTexture(GL_TEXTURE4);
	data.indices->Bind();
	glUniform1i(blurShad->Loc(7), 5);
	glActiveTexture(GL_TEXTURE5);
	data.edgeData->Bind();
	glUniform1i(blurShad->Loc(8), 6);
	glActiveTexture(GL_TEXTURE6);
	data.iconData->Bind();
	glUniform1i(blurShad->Loc(9), 7);
	glActiveTexture(GL_TEXTURE7);
	data.conData->Bind();
	glUniform2f(blurShad->Loc(10), 1, 0);
	glUniform1i(blurShad->Loc(11), (int)size);

	tmp->BindTarget();
	tmp->Clear(Color(0, 0), 1);
	GLUtils::DrawArrays(GL_TRIANGLES, 6);

	tar->BindTarget();
	tar->Clear(Color(0, 0), 1);
	glActiveTexture(GL_TEXTURE0);
	//tmp->Bind();
	glUniform2f(blurShad->Loc(10), 0, 1);
	GLUtils::DrawArrays(GL_TRIANGLES, 6);

	tar->UnbindTarget();

	blurShad->Unbind();
}


bool MeshSurfaceData::initd = false;
Shader MeshSurfaceData::uvInfoShad;
Shader MeshSurfaceData::uvInfoShad2;
Shader MeshSurfaceData::jmpInfoShad;

void MeshSurfaceData::InitShaders() {
	(uvInfoShad = Shader::New(glsl::uvinfoVert, glsl::uvinfoFrag))
		->AddUniforms({ "uvTex", "elemTex" });
	(uvInfoShad2 = Shader::New(glsl::minVert, glsl::uvinfoExpFrag))
		->AddUniforms({ "reso", "uvinfo", "uvcoords", "indices" });
	(jmpInfoShad = Shader::New(glsl::minVert, glsl::uvjmpgenFrag))
		->AddUniforms({ "uvinfo", "edgeData", "reso" });
	initd = true;
}

const MeshSurfaceData::infoTexSt& MeshSurfaceData::GetInfoTex(const Int2& res) {
	if (texs.count((uint64_t)res.x << 32 | res.y) == 0) return GenInfoTex(res);
	return texs[(uint64_t)res.x << 32 | res.y];
}

const MeshSurfaceData::infoTexSt& MeshSurfaceData::GenInfoTex(const Int2& res) {
	if (!initd) InitShaders();

	glViewport(0, 0, res.x, res.y);

	auto& tx = texs[(uint64_t)res.x << 32 | res.y];
	auto& uvInfoTex = tx.uvInfoTex;
	auto& jmpInfoTex = tx.jmpInfoTex;

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
	
	/*
	uvInfoShad2->Bind();
	
	glUniform2f(uvInfoShad2->Loc(0), res.x, res.y);
	glUniform1i(uvInfoShad2->Loc(1), 0);
	glActiveTexture(GL_TEXTURE0);
	uvInfoTex->tex(0)->Bind();
	glUniform1i(uvInfoShad2->Loc(2), 1);
	glActiveTexture(GL_TEXTURE1);
	uvcoords->Bind();
	glUniform1i(uvInfoShad2->Loc(3), 2);
	glActiveTexture(GL_TEXTURE2);
	indices->Bind();
	GLUtils::DrawArrays(GL_TRIANGLES, 6);
	
	uvInfoTex->Unbind();
	*/

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

	return tx;
}

CE_END_ED_NAMESPACE