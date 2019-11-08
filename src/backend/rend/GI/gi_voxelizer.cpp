#include "backend/chokoengine_backend.hpp"
#include "glsl/voxelFill.h"
#include "glsl/voxelDebug.h"

CE_BEGIN_BK_NAMESPACE

Shader GI::Voxelizer::voxShad;
Shader GI::Voxelizer::voxDebugShad;

RenderTarget3 GI::Voxelizer::res;

Mat4x4 GI::Voxelizer::lastVP;

GI::Voxelizer::regionSt GI::Voxelizer::regionOld;

GI::Voxelizer::regionSt GI::Voxelizer::region;

int GI::Voxelizer::resolution() {
	return !res ? 0 : res->width();
}

void GI::Voxelizer::resolution(int r) {
	if (!res || res->width() != r) {
		res = RenderTarget3::New(r, r, r, GL_RGBA8); //3x3x3 + padding (some cards do not support RGBA32UI)
	}
}

bool GI::Voxelizer::InitShaders() {
#ifdef PLATFORM_MAC
	return true; //
#else
	(voxShad = Shader::New(std::vector<std::string>{ glsl::voxelFillVert, glsl::voxelFillGeom, glsl::voxelFillFrag }, std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Geometry, ShaderType::Fragment }))
		->AddUniforms({ "_MVP", "layerCount" });

	(voxDebugShad = Shader::New(glsl::voxelDebugVert, glsl::voxelDebugFrag))
		->AddUniforms({ "num", "_VP", "tex" });

	resolution(128);

	return !!voxShad && !!voxDebugShad;
#endif
}

void GI::Voxelizer::Bake() {
#ifdef PLATFORM_MAC
	CE_NOT_IMPLEMENTED
#else
	lastVP = glm::ortho(region.x0, region.x1, region.y0, region.y1, region.z0, region.z1);

	res->BindTarget();
	float zero[4] = {};
	glClearBufferfv(GL_COLOR, 0, zero);
	res->UnbindTarget();

	voxShad->Bind();
	glUniform1i(voxShad->Loc(1), res->_width);
	glBindImageTexture(3, res->_pointer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	glBlendFunc(GL_ZERO, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);

	for (auto& rend : Renderer::orends) {
		const auto& mesh = rend->_mesh;
		if (!mesh) continue;
		const auto MVP = lastVP * rend->object()->transform()->worldMatrix();
		glUniformMatrix4fv(voxShad->Loc(0), 1, false, &MVP[0][0]);
		mesh->_vao->Bind();
		for (size_t a = 0; a < rend->_mesh->materialCount(); a++) {
			mesh->_elos[a]->Bind();
			glDrawElements(GL_TRIANGLES, mesh->_matTriangles[a].size() * 3, GL_UNSIGNED_INT, 0);
			mesh->_elos[a]->Unbind();
		}
		mesh->_vao->Unbind();
	}

	voxShad->Unbind();
#endif
}

void GI::Voxelizer::DrawDebug(const Mat4x4& vp) {
	const auto reso = res->_width;
	const auto mvp = vp * lastVP.inverse();

	voxDebugShad->Bind();

	glUniform1i(voxDebugShad->Loc(0), reso);
	glUniformMatrix4fv(voxDebugShad->Loc(1), 1, false, &mvp[0][0]);
	glUniform1i(voxDebugShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	res->Bind();

	UI::_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, reso * reso * reso * 36);
	UI::_vao->Unbind();

	voxDebugShad->Unbind();
}

CE_END_BK_NAMESPACE