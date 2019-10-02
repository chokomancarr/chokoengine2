#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/pointLightFrag.h"
#include "glsl/spotLightFrag.h"

CE_BEGIN_BK_NAMESPACE

bool Renderer::InitLightShaders() {
	(pointLightShad = Shader::New(glsl::minVert, glsl::pointLightFrag))
		->AddUniforms({ "_IP", "screenSize", "inGBuf0", "inGBuf1", "inGBuf2", "inGBufD", "lightPos", "lightStr", "lightRad", "lightDst", "lightCol", "falloff", "shadowMap", "shadowStr", "shadowBias" });

	(spotLightShad = Shader::New(glsl::minVert, glsl::spotLightFrag))
		->AddUniforms({ "_IP", "screenSize", "inGBuf0", "inGBuf1", "inGBuf2", "inGBufD", "lightPos", "lightDir", "lightStr", "lightRad", "lightDst", "lightAngleCos", "falloff", "shadowTex", "_LP", "shadowStr", "shadowBias" });
	return !!pointLightShad && !!spotLightShad;
}

void Renderer::RenderLight_Point(const Light& l, const FrameBuffer& gbuf, const Mat4x4& ip, const RenderTarget& tar) {
	const auto& pos = l->object()->transform()->worldPosition();

	if (l->_shadow) {
		RenderLight_Point_Shadow(l);
		tar->BindTarget();
		glViewport(0, 0, tar->_width, tar->_height);
	}

	pointLightShad->Bind();
	glUniformMatrix4fv(pointLightShad->Loc(0), 1, GL_FALSE, &ip[0][0]);
	glUniform2f(pointLightShad->Loc(1), tar->_width, tar->_height);
	glUniform1i(pointLightShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	gbuf->_texs[0]->Bind();
	glUniform1i(pointLightShad->Loc(3), 1);
	glActiveTexture(GL_TEXTURE1);
	gbuf->_texs[1]->Bind();
	glUniform1i(pointLightShad->Loc(4), 2);
	glActiveTexture(GL_TEXTURE2);
	gbuf->_texs[2]->Bind();
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
	glUniform1i(pointLightShad->Loc(12), 4);
	glActiveTexture(GL_TEXTURE4);
	if (l->_shadow) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, l->shadowBuffer_Cube->_depth->_pointer);
		glUniform1f(pointLightShad->Loc(13), l->_shadowStrength);
		glUniform1f(pointLightShad->Loc(14), l->_shadowBias);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glUniform1f(pointLightShad->Loc(13), 0);
	}

	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	pointLightShad->Unbind();
}

void Renderer::RenderLight_Point_Shadow(const Light& l) {
	if (l->shadowBuffer_Cube->_lastUpdated >= Time::millis()) {
		return;
	}
	l->shadowBuffer_Cube->_lastUpdated = Time::millis();

	const auto& _p = glm::perspectiveFov<float>(90 * Math::deg2rad, l->_shadowResolution, l->_shadowResolution, l->_radius, l->_distance);
	const Vec3 rs[] = {
		Vec3(180, 90, 0),
		Vec3(180, -90, 0),
		Vec3(-90, 0, 0),
		Vec3(90, 0, 0),
		Vec3(180, 0, 0),
		Vec3(180, 180, 0)
	};
	const auto& _t = Mat4x4::Translation(l->object()->transform()->worldPosition() * -1.f);

	glViewport(0, 0, l->_shadowResolution, l->_shadowResolution);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	for (int a = 0; a < 6; a++) {
		l->shadowBuffer_Cube->Bind((CubeMapFace)a);
		l->shadowBuffer_Cube->Clear();

		MVP::Switch(true);
		MVP::Clear();
		MVP::Mul(_p);
		MVP::Push();
		MVP::Mul(Mat4x4::Rotation(rs[a]));
		MVP::Push();
		MVP::Mul(_t);

		const auto& p = MVP::projection();

		for (auto& r : orends) {
			RenderMesh(r, p);
		}

		l->shadowBuffer_Cube->Unbind();
	}

	glBlendFunc(GL_ONE, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);
}

void Renderer::RenderLight_Spot(const Light& l, const FrameBuffer& gbuf, const Mat4x4& ip, const RenderTarget& tar) {
	const auto& pos = l->object()->transform()->worldPosition();
	const auto& fwd = l->object()->transform()->forward();

	Mat4x4 pShad;
	if (l->_shadow) {
		pShad = RenderLight_Spot_Shadow(l);
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
	if (l->_shadow) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, l->shadowBuffer_2D->_depth->_pointer);
		glUniformMatrix4fv(spotLightShad->Loc(14), 1, GL_FALSE, &pShad[0][0]);
		glUniform1f(spotLightShad->Loc(15), l->_shadowStrength);
		glUniform1f(spotLightShad->Loc(16), l->_shadowBias);
	}
	else {
		glUniform1f(spotLightShad->Loc(15), 0);
	}

	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	spotLightShad->Unbind();
}

Mat4x4 Renderer::RenderLight_Spot_Shadow(const Light& l) {
	if (l->shadowBuffer_2D->_lastUpdated >= Time::millis()) {
		return l->_lastShadowMatrix;
	}
	l->shadowBuffer_2D->_lastUpdated = Time::millis();

	l->shadowBuffer_2D->Bind();
	l->shadowBuffer_2D->Clear();

	const auto& p = l->_lastShadowMatrix = 
		glm::perspectiveFov<float>(l->_angle * 2 * Math::deg2rad, l->_shadowResolution, l->_shadowResolution, 0.1f, l->_distance)
		* Mat4x4::Rotation(Vec3(180, 0, 0))
		* l->object()->transform()->worldMatrix().inverse();

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