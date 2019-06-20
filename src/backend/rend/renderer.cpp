#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/skyFrag.h"

CE_BEGIN_BK_NAMESPACE

VertexObject Renderer::emptyVao;
Shader Renderer::skyShad;

void Renderer::ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras) {
    for (auto& o : oo) {
        auto c = o->GetComponent<Camera>();
        if (!!c) {
            cameras.push_back(c);
        }
        ScanObjects(o->children(), cameras);
    }
}

void Renderer::RenderSky(const Scene& scene, const Camera& cam) {
	const auto ip = glm::inverse(MVP::projection());

	const auto tar = cam->target();
	skyShad->Bind();
	glUniformMatrix4fv(skyShad->Loc(0), 1, GL_FALSE, &ip[0][0]);
	glUniform2f(skyShad->Loc(1), tar->_width, tar->_height);
	glUniform1i(skyShad->Loc(2), cam->orthographic());
	glUniform1i(skyShad->Loc(3), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene->sky()->_pointer);
	glUniform1f(skyShad->Loc(4), 1);
	emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	emptyVao->Unbind();
	skyShad->Unbind();
}

bool Renderer::Init() {
	emptyVao = std::make_shared<_VertexObject>();

	(skyShad = Shader::New(glsl::minVert, glsl::skyFrag))
		->AddUniforms({ "_IP", "screenSize", "isOrtho", "inSky", "skyStrength" });

	return true;
}

void Renderer::Render(const Scene& scene) {
    std::vector<Camera> cameras;

    ScanObjects(scene->objects(), cameras);

    for (auto& c : cameras) {
        RenderCamera(scene, c);
    }
}

void Renderer::RenderCamera(const Scene& scene, const Camera& cam) {
    MVP::Clear();
    
	const auto tar = cam->target();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, tar->_fbo);
	glViewport(0, 0, tar->_width, tar->_height);
	if ((cam->_clearType == CameraClearType::Color)
		|| (cam->_clearType == CameraClearType::ColorAndDepth))
		glClearBufferfv(GL_COLOR, 0, &cam->_clearColor[0]);
	if ((cam->_clearType == CameraClearType::Depth)
		|| (cam->_clearType == CameraClearType::ColorAndDepth))
	glClearBufferfv(GL_DEPTH, 0, &cam->_clearDepth);

	MVP::Switch(true);
	
	const auto& cwm = cam->object()->transform()->worldMatrix();
	
	MVP::Mul(cwm);
	MVP::Push();
	MVP::Mul(glm::perspectiveFov<float>(cam->fov() * Math::deg2rad, tar->_width, tar->_height, cam->nearClip(), cam->farClip()));

	RenderSky(scene, cam);

	glViewport(0, 0, Display::width(), Display::height());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

CE_END_BK_NAMESPACE