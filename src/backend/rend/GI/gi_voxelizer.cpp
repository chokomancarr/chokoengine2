#include "backend/chokoengine_backend.hpp"
#include "glsl/voxelFill.h"
#include "glsl/voxelDebug.h"

CE_BEGIN_BK_NAMESPACE

Shader GI::Voxelizer::voxShad;
Shader GI::Voxelizer::voxDebugShad;

RenderTarget3 GI::Voxelizer::res;

Mat4x4 GI::Voxelizer::lastV, GI::Voxelizer::lastVP;

int GI::Voxelizer::resolution() {
	return !res ? 0 : res->width();
}

void GI::Voxelizer::resolution(int r) {
	if (!res || res->width() != r) {
		res = RenderTarget3::New(r, r, r, GL_RG32UI); //4x4x4
	}
}

bool GI::Voxelizer::InitShaders() {
	(voxShad = Shader::New(std::vector<std::string>{ glsl::voxelFillVert, glsl::voxelFillGeom, glsl::voxelFillFrag }, std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Geometry, ShaderType::Fragment }))
		->AddUniforms({ "_MVP", "layerCount", "voxelTex" });

	(voxDebugShad = Shader::New(std::vector<std::string>{ glsl::voxelDebugVert, glsl::voxelDebugGeom, glsl::voxelDebugFrag }, std::vector<ShaderType>{ ShaderType::Vertex, ShaderType::Geometry, ShaderType::Fragment }))
		->AddUniforms({ "_P", "ptsz", "tex" });

	resolution(64);

	return !!voxShad && !!voxDebugShad;
}

void GI::Voxelizer::Bake(const Mat4x4& v, float sz) {
	const auto& p = glm::ortho(-sz / 2, sz / 2, -sz / 2, sz / 2, 0.f, sz);
	lastV = v;
	lastVP = p * v;
}

void GI::Voxelizer::DrawDebug(const Mat4x4& p) {
	const auto reso = res->width();
	const auto mvp = p * lastV.inverse();

	Vec4 v = lastV.inverse() * Vec4(0, 0, 0, 1);
	std::cout << v.x / v.w << ", " << v.y / v.w << ", " << v.z / v.w << std::endl;
	
	voxDebugShad->Bind();

	glUniformMatrix4fv(voxDebugShad->Loc(0), 1, false, &mvp[0][0]);
	glUniform1i(voxDebugShad->Loc(1), 0.01f);
	glUniform1i(voxDebugShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	res->Bind();

	UI::_vao->Bind();
	glDrawArrays(GL_POINTS, 0, reso * reso * reso);
	UI::_vao->Unbind();

	voxDebugShad->Unbind();
}

CE_END_BK_NAMESPACE