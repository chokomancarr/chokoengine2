#include "backend/chokoengine_backend.hpp"
#include "asset/texture_internal.hpp"
#include "glsl/voxelFill.h"
#include "glsl/voxelDownsample.h"
#include "glsl/voxelDebug.h"

CE_BEGIN_BK_NAMESPACE

Shader GI::Voxelizer::voxShad;
Shader GI::Voxelizer::voxDownShad;
Shader GI::Voxelizer::voxDebugShad;

GLuint GI::Voxelizer::occlusionTex = 0;
std::vector<GLuint> GI::Voxelizer::occlusionFbos = {};
std::vector<int> GI::Voxelizer::occlusionMipSzs = {};

int GI::Voxelizer::_reso = 0;
int GI::Voxelizer::_mips = 0;

Mat4x4 GI::Voxelizer::lastVP;

GI::Voxelizer::regionSt GI::Voxelizer::regionOld;

GI::Voxelizer::regionSt GI::Voxelizer::region;

int GI::Voxelizer::resolution() {
	return _reso;
}

void GI::Voxelizer::resolution(int r) {
	if (_reso != r) {
		_reso = r;
		if (!!occlusionTex) {
			glDeleteFramebuffers(_mips, &occlusionFbos[0]);
			occlusionFbos.clear();
			occlusionMipSzs.clear();
			glDeleteTextures(1, &occlusionTex);
		}

		glGenTextures(1, &occlusionTex);
		glBindTexture(GL_TEXTURE_3D, occlusionTex);

		std::vector<byte> data(r * r * r * 4);

		_mips = 0;
		while (r >= 4) {
			glTexImage3D(GL_TEXTURE_3D, _mips, GL_RGBA32F, r, r, r, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
			occlusionMipSzs.push_back(r);
			_mips++;
			r /= 2;
		}
		SetTexParams<GL_TEXTURE_3D>(_mips - 1, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);

		GLuint fbo;
		GLenum dbuf = GL_COLOR_ATTACHMENT0;
		for (int a = 0; a < _mips; a++) {
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture(GL_FRAMEBUFFER, dbuf, occlusionTex, a);
			glDrawBuffers(1, &dbuf);
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				Debug::Error("GI", "Could not create occlusion framebuffer: level " + std::to_string(a) + ", gl error " + std::to_string(status));
			}
			occlusionFbos.push_back(fbo);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindTexture(GL_TEXTURE_3D, 0);
	}
}

bool GI::Voxelizer::InitShaders() {
#ifdef PLATFORM_MAC
	return true; //
#else
	(voxShad = Shader::New(std::vector<std::string>{ glsl::voxelFillVert, glsl::voxelFillGeom, glsl::voxelFillFrag }, std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Geometry, ShaderType::Fragment }))
		->AddUniforms({ "_M", "_MVP", "layerCount" });

	(voxDownShad = Shader::New(std::vector<std::string>{ glsl::voxelDownsampleVert, glsl::voxelDownsampleGeom, glsl::voxelDownsampleFrag }, std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Geometry, ShaderType::Fragment }))
		->AddUniforms({ "sz", "tex", "mip" });

	(voxDebugShad = Shader::New(glsl::voxelDebugVert, glsl::voxelDebugFrag))
		->AddUniforms({ "num", "_VP", "occluTex", "mip" });

	float sz = 1;
	region = regionSt{ -sz, sz, -sz, sz, -sz, sz };
	resolution(32);

	return !!voxShad && !!voxDebugShad;
#endif
}

void GI::Voxelizer::Bake() {
#ifdef PLATFORM_MAC
	CE_NOT_IMPLEMENTED
#else
	lastVP = glm::ortho(region.x0, region.x1, region.y0, region.y1, region.z0, region.z1);

	glViewport(0, 0, _reso, _reso);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, occlusionFbos[0]);
	float zero[4] = {};
	glClearBufferfv(GL_COLOR, 0, zero);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	voxShad->Bind();
	glUniform1i(voxShad->Loc(2), _reso);
	glBindImageTexture(3, occlusionTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glBlendFunc(GL_ZERO, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);

	for (auto& rend : Renderer::orends) {
		const auto& mesh = rend->_mesh;
		if (!mesh) continue;
		const auto& M = rend->object()->transform()->worldMatrix();
		const auto MVP = lastVP * M;
		glUniformMatrix4fv(voxShad->Loc(0), 1, false, &M[0][0]);
		glUniformMatrix4fv(voxShad->Loc(1), 1, false, &MVP[0][0]);
		mesh->_vao->Bind();
		for (size_t a = 0; a < rend->_mesh->materialCount(); a++) {
			mesh->_elos[a]->Bind();
			glDrawElements(GL_TRIANGLES, mesh->_matTriangles[a].size() * 3, GL_UNSIGNED_INT, 0);
			mesh->_elos[a]->Unbind();
		}
		mesh->_vao->Unbind();
	}

	voxShad->Unbind();

	Downsample();
#endif
}

void GI::Voxelizer::Downsample() {
	glBlendFunc(GL_ONE, GL_ZERO);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	voxDownShad->Bind();
	glUniform1i(voxDownShad->Loc(1), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, occlusionTex);

	for (int a = 1; a < _mips; a++) {
		const auto tar = occlusionFbos[a];
		const auto sz = occlusionMipSzs[a];

		glViewport(0, 0, sz, sz);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, tar);

		glUniform1i(voxDownShad->Loc(0), sz);
		glUniform1f(voxDownShad->Loc(2), (float)(a - 1));

		UI::_vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6 * sz);
		UI::_vao->Unbind();
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	voxDownShad->Unbind();

	glEnable(GL_BLEND);
}

void GI::Voxelizer::DrawDebug(const Mat4x4& vp, int mip) {
	const auto sz = occlusionMipSzs[mip];
	const auto mvp = vp * lastVP.inverse();

	voxDebugShad->Bind();

	glUniform1i(voxDebugShad->Loc(0), sz);
	glUniformMatrix4fv(voxDebugShad->Loc(1), 1, false, &mvp[0][0]);
	glUniform1i(voxDebugShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, occlusionTex);
	glUniform1f(voxDebugShad->Loc(3), (float)mip);

	UI::_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, sz * sz * sz * 36);
	UI::_vao->Unbind();

	voxDebugShad->Unbind();
}

CE_END_BK_NAMESPACE