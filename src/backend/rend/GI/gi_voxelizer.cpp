#include "backend/chokoengine_backend.hpp"
#include "asset/texture_internal.hpp"
#include "glsl/voxelFill.h"
#include "glsl/voxelDownsample.h"
#include "glsl/voxelDebugAO.h"
#include "glsl/voxelDebugEm.h"

CE_BEGIN_BK_NAMESPACE

Shader GI::Voxelizer::voxShad;
Shader GI::Voxelizer::voxDownShad;
Shader GI::Voxelizer::voxDebugAOShad;
Shader GI::Voxelizer::voxDebugEmShad;

GLuint GI::Voxelizer::occlusionTex = 0;
std::vector<GLuint> GI::Voxelizer::occlusionFbos = {};
GLuint GI::Voxelizer::emissionTex[3] = {};
std::vector<GLuint> GI::Voxelizer::emissionFbos = {};

int GI::Voxelizer::_reso = 0;
int GI::Voxelizer::_mips = 0;

std::vector<int> GI::Voxelizer::mipSzs = {};

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
			glDeleteTextures(1, &occlusionTex);

			glDeleteFramebuffers(_mips, &emissionFbos[0]);
			emissionFbos.clear();
			glDeleteTextures(3, emissionTex);

			mipSzs.clear();
		}

		const std::vector<byte> data(r * r * r * 4);
		GLuint fbo;
		const GLenum dbuf[] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2
		};

		// ---- occlusion ----

		glGenTextures(1, &occlusionTex);
		glBindTexture(GL_TEXTURE_3D, occlusionTex);

		_mips = 0;
		while (r >= 4) {
			glTexImage3D(GL_TEXTURE_3D, _mips, GL_RGBA32F, r, r, r, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
			mipSzs.push_back(r);
			_mips++;
			r /= 2;
		}
		SetTexParams<GL_TEXTURE_3D>(_mips - 1, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
		glBindTexture(GL_TEXTURE_3D, 0);

		occlusionFbos.push_back(0);
		for (int a = 1; a < _mips; a++) {
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture(GL_FRAMEBUFFER, dbuf[0], occlusionTex, a);
			glDrawBuffers(1, dbuf);
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				Debug::Error("GI", "Could not create occlusion framebuffer: level " + std::to_string(a) + ", gl error " + std::to_string(status));
			}
			occlusionFbos.push_back(fbo);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// ---- emission ----

		glGenTextures(3, emissionTex);

		for (int a = 0; a < 3; a++) {
			glBindTexture(GL_TEXTURE_3D, emissionTex[a]);

			int mi = 0;
			for (auto& m : mipSzs) {
				glTexImage3D(GL_TEXTURE_3D, mi++, GL_RGBA32F, m, m, m, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
			}
			SetTexParams<GL_TEXTURE_3D>(_mips - 1, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
		}
		glBindTexture(GL_TEXTURE_3D, 0);

		for (int a = 0; a < _mips; a++) {
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			for (int b = 0; b < 3; b++)
			glFramebufferTexture(GL_FRAMEBUFFER, dbuf[b], emissionTex[b], a);
			glDrawBuffers(1, dbuf);
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				Debug::Error("GI", "Could not create emission framebuffer: level " + std::to_string(a) + ", gl error " + std::to_string(status));
			}
			emissionFbos.push_back(fbo);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glBindTexture(GL_TEXTURE_3D, 0);
	}
}

bool GI::Voxelizer::InitShaders() {
#ifdef PLATFORM_MAC
	return true; //
#else
	(voxShad = Shader::New(std::vector<std::string>{ glsl::voxelFillVert, glsl::voxelFillGeom, glsl::voxelFillFrag },
			std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Geometry, ShaderType::Fragment },
			std::vector<std::string>({ "emit_color", "emit_texture" })))
		->AddUniforms({ "_M", "_MVP", "layerCount", "emitStr", "emitCol", "emitTex" });

	(voxDownShad = Shader::New(std::vector<std::string>{ glsl::voxelDownsampleVert, glsl::voxelDownsampleGeom, glsl::voxelDownsampleFrag },
			std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Geometry, ShaderType::Fragment }))
		->AddUniforms({ "sz", "tex", "mip" });

	(voxDebugAOShad = Shader::New(glsl::voxelDebugAOVert, glsl::voxelDebugAOFrag))
		->AddUniforms({ "num", "_VP", "occluTex", "mip" });

	(voxDebugEmShad = Shader::New(std::vector<std::string>{ glsl::voxelDebugEmVert, glsl::voxelDebugEmFrag },
		std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Fragment }))
		->AddUniforms({ "num", "_VP", "emitTexX", "emitTexY", "emitTexZ", "mip" });

	float sz = 1.1f;
	region = regionSt{ -sz, sz, -sz, sz, -sz, sz };
	resolution(32);

	return !!voxShad && !!voxDebugAOShad && !!voxDebugEmShad;
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

	glBlendFunc(GL_ZERO, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);

	for (auto& rend : Renderer::orends) {
		const auto& mesh = rend->_mesh;
		if (!mesh) continue;
		const auto& M = rend->object()->transform()->worldMatrix();
		const auto MVP = lastVP * M;

		mesh->_vao->Bind();
		for (size_t a = 0; a < rend->_mesh->materialCount(); a++) {
			auto& mat = rend->_materials[a];
			auto& shad = mat->_shader;

			const bool emitCol = (shad->_giFlags & SHADER_GI_EMIT_COLOR) > 0;
			const bool emitTex = (shad->_giFlags & SHADER_GI_EMIT_TEXTURE) > 0;

			voxShad->SetOption("emit_color", emitCol);
			voxShad->SetOption("emit_texture", emitTex);
			voxShad->Bind();

			glUniformMatrix4fv(voxShad->Loc(0), 1, false, &M[0][0]);
			glUniformMatrix4fv(voxShad->Loc(1), 1, false, &MVP[0][0]);
			glUniform1i(voxShad->Loc(2), _reso);
			if (emitCol || emitTex) {
				glUniform1f(voxShad->Loc(3), mat->GetGIEmissionStr());
				if (emitCol) {
					Color c = mat->GetGIEmissionCol();
					glUniform3f(voxShad->Loc(4), c.r, c.g, c.b);
				}
				if (emitTex) {
					glUniform1i(voxShad->Loc(5), 0);
					glActiveTexture(GL_TEXTURE0);
					mat->GetGIEmissionTex()->Bind();
				}
			}
			glBindImageTexture(3, occlusionTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			for (int e = 0; e < 3; e++) {
				glBindImageTexture(4 + e, emissionTex[e], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			}

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
		const auto sz = mipSzs[a];

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

void GI::Voxelizer::DrawDebugAO(const Mat4x4& vp, int mip) {
	const auto sz = mipSzs[mip];
	const auto mvp = vp * lastVP.inverse();

	voxDebugAOShad->Bind();

	glUniform1i(voxDebugAOShad->Loc(0), sz);
	glUniformMatrix4fv(voxDebugAOShad->Loc(1), 1, false, &mvp[0][0]);
	glUniform1i(voxDebugAOShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, occlusionTex);
	glUniform1f(voxDebugAOShad->Loc(3), (float)mip);

	UI::_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, sz * sz * sz * 36);
	UI::_vao->Unbind();

	voxDebugAOShad->Unbind();
}

void GI::Voxelizer::DrawDebugEm(const Mat4x4& vp, int mip) {
	const auto sz = mipSzs[mip];
	const auto mvp = vp * lastVP.inverse();

	voxDebugEmShad->Bind();

	glUniform1i(voxDebugEmShad->Loc(0), sz);
	glUniformMatrix4fv(voxDebugEmShad->Loc(1), 1, false, &mvp[0][0]);
	glUniform1i(voxDebugEmShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, emissionTex[0]);
	glUniform1i(voxDebugEmShad->Loc(3), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, emissionTex[1]);
	glUniform1i(voxDebugEmShad->Loc(4), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, emissionTex[2]);
	glUniform1f(voxDebugEmShad->Loc(5), (float)mip);

	UI::_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, sz * sz * sz * 36);
	UI::_vao->Unbind();

	voxDebugEmShad->Unbind();
}

CE_END_BK_NAMESPACE