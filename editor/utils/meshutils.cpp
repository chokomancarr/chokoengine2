#include "chokoeditor.hpp"
#include "meshutils.hpp"
#include "glutils.hpp"
#include "glsl/minVert.h"
#include "glsl/padTextureFrag.h"
#include "glsl/surfaceBlurFrag.h"
#include "glsl/uvinfo.h"
#include "glsl/uvinfo_exp.h"
#include "glsl/uvjmpgen.h"
#include <iomanip>

CE_BEGIN_ED_NAMESPACE

bool MeshUtils::initd = false;

Shader MeshUtils::padShad;
Shader MeshUtils::blurShad;

void MeshUtils::Init() {
	(padShad = Shader::New(glsl::minVert, glsl::padTextureFrag))
		->AddUniforms({ "reso", "colTex", "infoTex" });
	(blurShad = Shader::New(glsl::minVert, glsl::surfBlurFrag))
		->AddUniforms({
			"reso", "colTex", "infoTex", "matTex", "vecTex", "sclTex", "dir0"
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

	data.vertCount = m->vertexCount();
	data.indCount = m->triangleCount();

	std::vector<glm::mat2x2> mTex2Tri(data.indCount);
	std::vector<glm::mat2x3> mTri2Mesh(data.indCount);
	std::vector<glm::mat3x3> mAlignPlane(data.indCount * 3);
	std::vector<glm::mat3x2> mMesh2Tri(data.indCount);
	std::vector<glm::mat2x2> mTri2Tex(data.indCount);
	std::vector<glm::mat2x2> mTransform(data.indCount * 3);
	std::vector<glm::vec2> mScales(data.indCount);

	const auto& poss = m->positions();
	const auto& uvs = m->texcoords();
	const auto& inds = m->triangles();

	//--------- mesh buffers -----------
	data.positions = TextureBuffer::New(
		VertexBuffer_New(true, 3, data.vertCount, m->positions().data()),
		GL_RGB32F);
	data.uvcoords = TextureBuffer::New(
		VertexBuffer_New(true, 2, data.vertCount, m->texcoords().data()),
		GL_RG32F);
	data.indices = TextureBuffer::New(
		VertexBuffer_New(false, 3, data.indCount, m->triangles().data()),
		GL_RGB32I);

	//unique vert indices
	int uvcnt = 1;
	std::vector<int> i2v(data.vertCount, 0); //index -> unique index
	std::unordered_map<std::array<float, 3>, int> _map;

	for (int a = 0; a < data.vertCount; a++) {
		const auto& va = poss[a];
		std::array<float, 3> ar;
		std::memcpy(ar.data(), &va.x, sizeof(float) * 3);
		auto& res = _map[ar];
		if (!res) {
			res = a + 1;
			uvcnt++;
		}
		i2v[a] = res - 1;
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
	std::vector<Int4> icons(data.indCount * 3, Int4(-1));

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
				icons[k] = Int4(Int3(res), res.w);
				icons[res.w] = Int4(ts[es[b][1]], ts[es[b][0]], ts[es[b][2]], k);
			}
			else if (tvst.count(COMB(vt2, vt1)) == 1) {
				const auto& res = tvst[COMB(vt2, vt1)];
				icons[k] = Int4(Int3(res), res.w);
				icons[res.w] = Int4(ts[es[b][1]], ts[es[b][0]], ts[es[b][2]], k);
			}
			else {
				tvst[COMB(vt1, vt2)] = Int4(ts[es[b][1]], ts[es[b][0]], ts[es[b][2]], k);
			}
		}
	}

	data.iconData = TextureBuffer::New(
		VertexBuffer_New(false, 4, data.indCount * 3, icons.data()),
		GL_RGBA32I);

	//--------- transform matrices -----------

#if 0

#define _m(v) (v >= 0 ? " " : "") << v

	const auto printm2 = [](std::string nm, glm::mat2x2 m) {
		std::cout << nm << std::fixed << std::setprecision(5) << " = [ " << _m(m[0][0]) << " , " << _m(m[1][0]) << "\n"
			<< std::string(nm.length() + 5, ' ') << _m(m[0][1]) << " , " << _m(m[1][1]) << " ]" << std::endl;
	};
	const auto printm32 = [](std::string nm, glm::mat3x2 m) {
		std::cout << nm << std::fixed << std::setprecision(5) << " = [ " << _m(m[0][0]) << " , " << _m(m[1][0]) << " , " << _m(m[2][0]) << "\n"
			<< std::string(nm.length() + 5, ' ') << _m(m[0][1]) << " , " << _m(m[1][1]) << " , " << _m(m[2][1]) << " ]" << std::endl;
	};
	const auto printm23 = [](std::string nm, glm::mat2x3 m) {
		std::cout << nm << std::fixed << std::setprecision(5) << " = [ " << _m(m[0][0]) << " , " << _m(m[1][0]) << "\n"
			<< std::string(nm.length() + 5, ' ') << _m(m[0][1]) << " , " << _m(m[1][1]) << "\n"
			<< std::string(nm.length() + 5, ' ') << _m(m[0][2]) << " , " << _m(m[1][2]) << " ]" << std::endl;
	};
	const auto printm3 = [](std::string nm, glm::mat3x3 m) {
		std::cout << nm << std::fixed << std::setprecision(5) << " = [ " << _m(m[0][0]) << " , " << _m(m[1][0]) << " , " << _m(m[2][0]) << "\n"
			<< std::string(nm.length() + 5, ' ') << _m(m[0][1]) << " , " << _m(m[1][1]) << " , " << _m(m[2][1]) << "\n"
			<< std::string(nm.length() + 5, ' ') << _m(m[0][2]) << " , " << _m(m[1][2]) << " , " << _m(m[2][2]) << " ]" << std::endl;
	};

#undef _m

#endif

	glm::vec2 sclsum(0, 0);
	int sclsumw = 0;

	for (auto i = 0; i < data.indCount; i++) {
		const auto& v = edata[i*3];
		mTri2Tex[i] = glm::mat2x2(v.x, v.y, v.z, v.w);
		mTex2Tri[i] = glm::inverse(mTri2Tex[i]);
		const auto vm1 = (glm::vec3)edata[i*3 + 1];
		const auto vm2 = (glm::vec3)edata[i*3 + 2];
		const auto vm3 = glm::cross(vm1, vm2);
		mTri2Mesh[i] = glm::mat2x3(vm1, vm2);
		const auto m3 = glm::mat3x3(vm1, vm2, vm3);
		mMesh2Tri[i] = (glm::mat3x2)glm::inverse(m3);
		
		const glm::vec3 axes[3] = {
			vm1, vm2 - vm1, -vm2
		};
		for (int e = 0; e < 3; e++) {
			auto ic = icons[i * 3 + e];
			int c = ic.z;
			if (c == -1) continue;
			int j = ic.w / 3;
			if (j > i) continue;
			int f = ic.w - j * 3;
			const auto& x = axes[e];
			const auto& xp = *(glm::vec3*)&m->positions()[m->triangles()[i][e]];
			const auto xc = (
				m->positions()[m->triangles()[i][0]] +
				m->positions()[m->triangles()[i][1]] +
				m->positions()[m->triangles()[i][2]]
			) * 0.333333f;
			const auto tp = *(glm::vec3*)&m->positions()[c];
			auto tmp = glm::cross(tp - xp, x);
			auto td = normalize(glm::cross(x, tmp));
			tmp = glm::cross(x, *(glm::vec3*)&xc - xp);
			auto ts = normalize(glm::cross(x, tmp));
			float ca = glm::clamp(glm::dot(ts, td), -0.99999999f, 0.9999999f);
			float a = std::acos(ca) * 180 / 3.14159f;
			if (dot(tmp, td) < 0) a = 360 - a;
			auto q = Quat::FromAxisAngle(*(Vec3*)&x, a);
			mAlignPlane[i * 3 + e] = glm::mat3_cast(*(glm::quat*)&q);
			mTransform[j * 3 + f] = glm::inverse(
				mTransform[i * 3 + e] = mTri2Tex[j] * mMesh2Tri[j] * mAlignPlane[i * 3 + e] * mTri2Mesh[i] * mTex2Tri[i]
			);
		}

		glm::vec3 rx = mTri2Mesh[i] * mTex2Tri[i] * glm::vec2(1.f, 0.f);
		glm::vec3 ry = mTri2Mesh[i] * mTex2Tri[i] * glm::vec2(0.f, 1.f);
		mScales[i] = glm::vec2(1.f / glm::length(rx), 1.f / glm::length(ry));
		if (mScales[i].x == mScales[i].x && mScales[i].y == mScales[i].y) {
			sclsum += mScales[i];
			sclsumw ++;
		}
	}

	sclsum /= sclsumw;
	std::cout << "scale = " << sclsum.x << ", " << sclsum.y << std::endl;
	for (auto& s : mScales) {
		s /= sclsum;
	}

	data.uvMats = TextureBuffer::New(
		VertexBuffer_New(true, 4, data.indCount * 3, mTransform.data()),
		GL_RGBA32F);

	data.scaleData = TextureBuffer::New(
		VertexBuffer_New(true, 2, data.indCount * 3, mScales.data()),
		GL_RG32F);

	std::vector<Int4> angles(data.indCount);
	std::vector<Int4> vecdata(data.indCount);

#define TOI(f) ((int)((f) * 100000))

	for (auto i = 0; i < data.indCount; i++) {
		Int4& res = angles[i];

		Vec2 pts[3];
		Vec2 cent(0);
		for (int e = 0; e < 3; e++) {
			cent += (pts[e] = m->texcoords()[m->triangles()[i][e]]);
		}
		cent *= 0.33333f;

		res.x = TOI(cent.x);
		res.y = TOI(cent.y);

		float pta[3];

		for (int e = 0; e < 3; e++) {
			pts[e] = (pts[e] - cent).normalized();
			pta[e] = std::acos(pts[e].x);
			if (pts[e].y < 0) pta[e] = 2 * 3.14159f - pta[e];

			if (e != 0) {
				pta[e] -= pta[0];
				if (pta[e] < 0) pta[e] += 2 * 3.14159f;
			}
		}

		int tmp = TOI(pta[2]);
		res.z = (TOI(pta[0]) << 10) | (tmp >> 10);
		res.w = (TOI(pta[1]) << 10) | (tmp & ((1 << 10) - 1));

		
#define encode16(v) int(v.x * 20000) << 16 | int(v.y * 20000)
#define encode16n(v) int(v.x * 10000 + 10000) << 16 | int(v.y * 10000 + 10000)

		auto& vec = vecdata[i];
		vec.x = encode16(cent);
		vec.y = encode16n(pts[0]);
		vec.z = encode16n(pts[1]);
		vec.w = encode16n(pts[2]);
	}

	data.angleData = TextureBuffer::New(
		VertexBuffer_New(false, 4, data.indCount, angles.data()),
		GL_RGBA32I);

	data.vecData = TextureBuffer::New(
		VertexBuffer_New(false, 4, data.indCount, vecdata.data()),
		GL_RGBA32I
	);

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
	glUniform1i(blurShad->Loc(1), 0);
	glActiveTexture(GL_TEXTURE0);
	src->Bind();
	glUniform1i(blurShad->Loc(2), 1);
	glActiveTexture(GL_TEXTURE1);
	info.jmpInfoTex->tex(0)->Bind();
	glUniform1i(blurShad->Loc(3), 2);
	glActiveTexture(GL_TEXTURE2);
	data.uvMats->Bind();
	glUniform1i(blurShad->Loc(4), 3);
	glActiveTexture(GL_TEXTURE3);
	data.vecData->Bind();
	glUniform1i(blurShad->Loc(5), 4);
	glActiveTexture(GL_TEXTURE4);
	data.scaleData->Bind();
	glUniform2f(blurShad->Loc(6), size, 0);

	tmp->BindTarget();
	tmp->Clear(Color(0, 0), 1);
	GLUtils::DrawArrays(GL_TRIANGLES, 6);

	tar->BindTarget();
	tar->Clear(Color(0, 0), 1);
	glActiveTexture(GL_TEXTURE0);
	tmp->Bind();
	glUniform2f(blurShad->Loc(6), 0, size);
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
		->AddUniforms({ "reso", "indices", "uvcoords", "uvinfo", "iconData" });
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
	uvInfoShad->Unbind();

	jmpInfoTex = FrameBuffer_New(res.x, res.y, { GL_RGBA32I });
	jmpInfoTex->Bind();
	jmpInfoTex->Clear();

	jmpInfoShad->Bind();

	glUniform2f(jmpInfoShad->Loc(0), res.x, res.y);
	glUniform1i(jmpInfoShad->Loc(1), 0);
	glActiveTexture(GL_TEXTURE0);
	indices->Bind();
	glUniform1i(jmpInfoShad->Loc(2), 1);
	glActiveTexture(GL_TEXTURE1);
	uvcoords->Bind();
	glUniform1i(jmpInfoShad->Loc(3), 2);
	glActiveTexture(GL_TEXTURE2);
	uvInfoTex->tex(0)->Bind();
	glUniform1i(jmpInfoShad->Loc(4), 3);
	glActiveTexture(GL_TEXTURE3);
	iconData->Bind();
	GLUtils::DrawArrays(GL_TRIANGLES, 6);

	jmpInfoShad->Unbind();
	jmpInfoTex->Unbind();

	return tx;
}

CE_END_ED_NAMESPACE