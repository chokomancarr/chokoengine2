#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/skyFrag.h"

CE_BEGIN_BK_NAMESPACE

VertexObject Renderer::emptyVao;
Shader Renderer::skyShad;

void Renderer::ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras,
		std::vector<MeshRenderer>& rends) {
    for (auto& o : oo) {
        auto c = o->GetComponent<Camera>();
        if (!!c) {
            cameras.push_back(c);
        }
		auto r = o->GetComponent<MeshRenderer>();
		if (!!r) {
			rends.push_back(r);
		}
        ScanObjects(o->children(), cameras, rends);
    }
}

void Renderer::RenderSky(const Scene& scene, const Camera& cam) {
	const auto ip = glm::inverse(MVP::projection());

	const auto& tar = cam->_target;
	const auto& gbuf = cam->_deferredBuffer;
	skyShad->Bind();
	glUniformMatrix4fv(skyShad->Loc(0), 1, GL_FALSE, &ip[0][0]);
	glUniform2f(skyShad->Loc(1), tar->_width, tar->_height);
	glUniform1i(skyShad->Loc(2), cam->orthographic());
	glUniform1i(skyShad->Loc(3), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[0]->_pointer);
	glUniform1i(skyShad->Loc(4), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[1]->_pointer);
	glUniform1i(skyShad->Loc(5), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuf->_depth->_pointer);
	glUniform1i(skyShad->Loc(6), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, scene->sky()->_pointer);
	glUniform1f(skyShad->Loc(7), 1);
	emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	emptyVao->Unbind();
	skyShad->Unbind();
}

bool Renderer::Init() {
	emptyVao = std::make_shared<_VertexObject>();

	(skyShad = Shader::New(glsl::minVert, glsl::skyFrag))
		->AddUniforms({ "_IP", "screenSize", "isOrtho", "inColor", "inNormal", "inDepth", "inSky", "skyStrength" });

	return true;
}

void Renderer::Render(const Scene& scene) {
    std::vector<Camera> cameras;
	std::vector<MeshRenderer> rends;

	ScanObjects(scene->objects(), cameras, rends);

    for (auto& c : cameras) {
        RenderCamera(scene, c, rends);
    }
}

void Renderer::RenderCamera(const Scene& scene, const Camera& cam, const std::vector<MeshRenderer> rends) {
	const auto& tar = cam->target();

    MVP::Clear();
	MVP::Switch(true);
	const auto& cwm = cam->object()->transform()->worldMatrix();
	MVP::Mul(glm::perspectiveFov<float>(cam->fov() * Math::deg2rad, tar->_width, tar->_height, cam->nearClip(), cam->farClip()));
	MVP::Push();
	MVP::Mul(cwm);

	glViewport(0, 0, tar->_width, tar->_height);

	auto& gbuf = cam->_deferredBuffer;
	if (!gbuf) {
		gbuf = FrameBuffer_New(tar->_width, tar->_height, 4);
	}
	gbuf->Bind();
	gbuf->Clear();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	for (auto& r : rends) {
		r->_mesh->_vao->Bind();
		for (size_t a = 0; a < r->_mesh->materialCount(); a++) {
			if (!r->_materials[a]) continue;
			r->_materials[a]->SetUniform("_MV", MVP::modelview());
			r->_materials[a]->SetUniform("_P", MVP::projection());
			r->_materials[a]->SetUniform("_MVP", MVP::projection() * MVP::modelview());
			r->_materials[a]->Bind();
			r->_mesh->_elos[a]->Bind();
			glDrawElements(GL_TRIANGLES, r->_mesh->_matTriangles[a].size() * 3, GL_UNSIGNED_INT, 0);
			r->_mesh->_elos[a]->Unbind();
			r->_materials[a]->Unbind();
		}
		r->_mesh->_vao->Unbind();
	}

	gbuf->Unbind();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glBlendFunc(GL_ONE, GL_ONE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, tar->_fbo);

	if ((cam->_clearType == CameraClearType::Color)
		|| (cam->_clearType == CameraClearType::ColorAndDepth))
		glClearBufferfv(GL_COLOR, 0, &cam->_clearColor[0]);
	if ((cam->_clearType == CameraClearType::Depth)
		|| (cam->_clearType == CameraClearType::ColorAndDepth))
		glClearBufferfv(GL_DEPTH, 0, &cam->_clearDepth);

	RenderSky(scene, cam);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glViewport(0, 0, Display::width(), Display::height());
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

CE_END_BK_NAMESPACE