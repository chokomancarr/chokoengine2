#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/pointLightFrag.h"
#include "glsl/spotLightFrag.h"

CE_BEGIN_BK_NAMESPACE

bool Renderer::InitLightShaders() {
	(pointLightShad = Shader::New(glsl::minVert, glsl::pointLightFrag))
		->AddUniforms({ "_IP", "screenSize", "inGBuf0", "inGBuf1", "inGBuf2", "inGBufD", "lightPos", "lightStr", "lightRad", "lightDst", "lightCol", "falloff" });

	(spotLightShad = Shader::New(glsl::minVert, glsl::spotLightFrag))
		->AddUniforms({ "_IP", "screenSize", "inGBuf0", "inGBuf1", "inGBuf2", "inGBufD", "lightPos", "lightDir", "lightStr", "lightRad", "lightDst", "lightAngleCos", "falloff" });
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
	glUniform1i(pointLightShad->Loc(11), (int)l->falloff());

	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	pointLightShad->Unbind();
}

void Renderer::RenderLight_Spot(const Light& l, const Camera& cam) {
	const auto ip = glm::inverse(MVP::projection());
	const auto& pos = l->object()->transform()->worldPosition();
	const auto& fwd = Vec3(1, -1, 1).normalized();//l->object()->transform()->forward();
	const auto& tar = cam->_target;
	const auto& gbuf = cam->_deferredBuffer;

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
	glUniform1f(spotLightShad->Loc(8), l->strength());
	glUniform1f(spotLightShad->Loc(9), l->radius());
	glUniform1f(spotLightShad->Loc(10), l->distance());
	glUniform1f(spotLightShad->Loc(11), std::cos(30.f));
	glUniform1i(spotLightShad->Loc(12), (int)l->falloff());

	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	spotLightShad->Unbind();
}

void Renderer::RenderLight_Directional(const Light& l, const Camera& cam) {
	CE_NOT_IMPLEMENTED;
}

CE_END_BK_NAMESPACE