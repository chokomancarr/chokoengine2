#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

void Renderer::ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras) {
    for (auto& o : oo) {
        auto c = o->GetComponent<Camera>();
        if (!!c) {
            cameras.push_back(c);
        }
        ScanObjects(o->children(), cameras);
    }
}

void Renderer::Render(const Scene& scene) {
    static std::vector<Camera> cameras;

    ScanObjects(scene->objects(), cameras);

    for (auto& c : cameras) {
        RenderCamera(c);
    }
}

void Renderer::RenderCamera(const Camera& cam) {
    MVP::Clear();
    
	auto& tar = cam->target();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, tar->_fbo);
	glViewport(0, 0, tar->_width, tar->_height);
	if ((cam->_clearType == CameraClearType::Color)
		|| (cam->_clearType == CameraClearType::ColorAndDepth))
		glClearBufferfv(GL_COLOR, 0, &cam->_clearColor[0]);
	if ((cam->_clearType == CameraClearType::Depth)
		|| (cam->_clearType == CameraClearType::ColorAndDepth))
	glClearBufferfv(GL_DEPTH, 0, &cam->_clearDepth);
	glViewport(0, 0, Display::width(), Display::height());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

CE_END_BK_NAMESPACE