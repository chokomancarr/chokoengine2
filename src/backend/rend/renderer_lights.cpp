#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/pointLightFrag.h"
#include "glsl/spotLightFrag.h"

CE_BEGIN_BK_NAMESPACE

bool Renderer::InitLightShaders() {
	(pointLightShad = Shader::New(glsl::minVert, glsl::pointLightFrag))
		->AddUniforms({ "_IP", "screenSize", "inGBuf0", "inGBuf1", "inGBuf2", "inGBufD", "lightPos", "lightStr", "lightRad", "lightDst", "lightCol", "falloff" });

	(spotLightShad = Shader::New(glsl::minVert, glsl::spotLightFrag))
		->AddUniforms({ "_IP", "screenSize", "inGBuf0", "inGBuf1", "inGBuf2", "inGBufD", "lightPos", "lightDir", "lightStr", "lightRad", "lightDst", "lightAngleCos", "falloff", "shadowTex", "_LP", "shadowStr", "shadowBias" });
	return !!pointLightShad && !!spotLightShad;
}

void Renderer::RenderLight_Point(const Light& l, const Camera& cam) {
	const auto ip = glm::inverse(MVP::projection());
	const auto& pos = l->object()->transform()->worldPosition();
	const auto& tar = cam->_target;
	const auto& gbuf = cam->_deferredBuffer;

	pointLightShad->Bind();
	glUniformMatrix4fv(pointLightShad->Loc(0), 1, GL_FALSE, &ip[0][0]);
	glUniform2f(pointLightShad->Loc(1), tar->_width, tar->_height);
	glUniform1i(pointLightShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[0]->_pointer);
	glUniform1i(pointLightShad->Loc(3), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[1]->_pointer);
	glUniform1i(pointLightShad->Loc(4), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[2]->_pointer);
	glUniform1i(pointLightShad->Loc(5), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuf->_depth->_pointer);
	glUniform3f(pointLightShad->Loc(6), pos.x, pos.y, pos.z);
	glUniform1f(pointLightShad->Loc(7), l->strength());
	glUniform1f(pointLightShad->Loc(8), l->radius());
	glUniform1f(pointLightShad->Loc(9), l->distance());
	const auto& col = l->color();
	glUniform3f(pointLightShad->Loc(10), col.r, col.g, col.b);
	glUniform1i(pointLightShad->Loc(11), (int)l->_falloff);

	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	pointLightShad->Unbind();
}

void Renderer::RenderLight_Spot(const Light& l, const Camera& cam, const Mat4x4& ip, const RenderTarget& tar, const std::vector<MeshRenderer>& orends) {
	const auto& pos = l->object()->transform()->worldPosition();
	const auto& fwd = l->object()->transform()->forward();
	const auto& gbuf = cam->_deferredBuffer;

	Mat4x4 pShad;
	if (l->_shadow) {
		pShad = RenderLight_Spot_Shadow(l, orends);
		tar->BindTarget();
		glViewport(0, 0, tar->_width, tar->_height);
	}

	spotLightShad->Bind();
	glUniformMatrix4fv(spotLightShad->Loc(0), 1, GL_FALSE, &ip[0][0]);
	glUniform2f(spotLightShad->Loc(1), tar->_width, tar->_height);
	glUniform1i(spotLightShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[0]->_pointer);
	glUniform1i(spotLightShad->Loc(3), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[1]->_pointer);
	glUniform1i(spotLightShad->Loc(4), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[2]->_pointer);
	glUniform1i(spotLightShad->Loc(5), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuf->_depth->_pointer);
	glUniform3f(spotLightShad->Loc(6), pos.x, pos.y, pos.z);
	glUniform3f(spotLightShad->Loc(7), fwd.x, fwd.y, fwd.z);
	glUniform1f(spotLightShad->Loc(8), l->_strength);
	glUniform1f(spotLightShad->Loc(9), l->_radius);
	glUniform1f(spotLightShad->Loc(10), l->_distance);
	glUniform1f(spotLightShad->Loc(11), std::cos(l->_angle * Math::deg2rad));
	glUniform1i(spotLightShad->Loc(12), (int)l->_falloff);
	glUniform1i(spotLightShad->Loc(13), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, l->shadowBuffer_2D->_depth->_pointer);
	glUniformMatrix4fv(spotLightShad->Loc(14), 1, GL_FALSE, &pShad[0][0]);
	glUniform1f(spotLightShad->Loc(15), l->_shadowStrength);
	glUniform1f(spotLightShad->Loc(16), l->_shadowBias);

	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	spotLightShad->Unbind();
}

Mat4x4 Renderer::RenderLight_Spot_Shadow(const Light& l, const std::vector<MeshRenderer>& orends) {
	l->shadowBuffer_2D->Bind();
	l->shadowBuffer_2D->Clear();

	MVP::Switch(true);
	MVP::Clear();
	MVP::Mul(glm::perspectiveFov<float>(l->_angle * 2 * Math::deg2rad, l->_shadowResolution, l->_shadowResolution, 0.1f, l->_distance));
	MVP::Push();
	MVP::Mul(Mat4x4::Rotation(Vec3(180, 0, 0)));
	MVP::Push();
	MVP::Mul(l->object()->transform()->worldMatrix().inverse());

	const auto& p = MVP::projection();

	glViewport(0, 0, l->_shadowResolution, l->_shadowResolution);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	for (auto& r : orends) {
		RenderMesh(r, p);
	}

	glBlendFunc(GL_ONE, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);

	l->shadowBuffer_2D->Unbind();

	return p;
}

void Renderer::RenderLight_Directional(const Light& l, const Camera& cam) {
	CE_NOT_IMPLEMENTED;
}

CE_END_BK_NAMESPACE