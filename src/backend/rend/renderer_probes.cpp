#include "backend/chokoengine_backend.hpp"
#include "ext/glmext.hpp"

CE_BEGIN_BK_NAMESPACE

void Renderer::RenderLightProbe(const LightProbe& lp) {
	const auto& res = lp->_resolution;
	const auto& _p = glm::perspectiveFov<float>(90 * Math::deg2rad, res, res, lp->_nearClip, lp->_farClip);
	const Vec3 rs[] = {
		Vec3(180, 90, 0),
		Vec3(180, -90, 0),
		Vec3(-90, 0, 0),
		Vec3(90, 0, 0),
		Vec3(180, 0, 0),
		Vec3(180, 180, 0)
	};
	const auto& _t = Mat4x4::Translation(lp->object()->transform()->worldPosition() * -1.f);

	auto& gbuf = lp->_deferredBuffer;
	if (!gbuf) {
		gbuf = FrameBuffer_New(res, res, {
			GL_RGBA, GL_RGB32F, GL_RGBA, GL_RGBA
		});
	}
	
	for (int a = 0; a < 6; a++) {
		auto& tar = lp->_fbo->_pointers[a];

		const auto& p = glm_cast(_p) * Mat4x4::Rotation(rs[a]) * _t;

		RenderScene(tar, lp->_fbo->_tmpPointer, p, gbuf, [&]() {
			if ((lp->_clearType == CameraClearType::Color)
					|| (lp->_clearType == CameraClearType::ColorAndDepth)) {
				glClearBufferfv(GL_COLOR, 0, &lp->_clearColor.r);
			}
			if ((lp->_clearType == CameraClearType::Depth)
				|| (lp->_clearType == CameraClearType::ColorAndDepth))
				glClearBufferfv(GL_DEPTH, 0, &lp->_clearDepth);
		}, false);
	}

	lp->_fbo->_maps[0]->ComputeGlossMipmaps();

	glViewport(0, 0, Display::width(), Display::height());
}

void Renderer::ApplyLightProbe(const LightProbe& lp, int w, int h, const FrameBuffer& gbuf, const Mat4x4& ip) {
	probeShad->Bind();
	glUniformMatrix4fv(probeShad->Loc(0), 1, GL_FALSE, fptr(ip));
	glUniform2f(probeShad->Loc(1), w, h);
	glUniform1i(probeShad->Loc(2), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[0]->_pointer);
	glUniform1i(probeShad->Loc(3), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[1]->_pointer);
	glUniform1i(probeShad->Loc(4), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[2]->_pointer);
	glUniform1i(probeShad->Loc(5), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[3]->_pointer);
	glUniform1i(probeShad->Loc(6), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gbuf->_depth->_pointer);
	glUniform1i(probeShad->Loc(7), 5);
	glActiveTexture(GL_TEXTURE5);
	lp->_fbo->_maps[0]->Bind();
	glUniform1f(probeShad->Loc(8), lp->_strength);
	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	probeShad->Unbind();
}

CE_END_BK_NAMESPACE