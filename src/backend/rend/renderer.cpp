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
	std::vector<MeshRenderer> rends;

	ScanObjects(scene->objects(), cameras, rends);

    for (auto& c : cameras) {
        RenderCamera(scene, c, rends);
    }
}

void Renderer::RenderCamera(const Scene& scene, const Camera& cam, const std::vector<MeshRenderer> rends) {
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
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	MVP::Mul(glm::perspectiveFov<float>(cam->fov() * Math::deg2rad, tar->_width, tar->_height, cam->nearClip(), cam->farClip()));
	MVP::Push();
	MVP::Mul(cwm);
	for (auto& r : rends) {
		r->_mesh->_vao->Bind();
		for (size_t a = 0; a < r->_mesh->materialCount(); a++) {
			if (!r->_materials[a]) continue;
			r->_materials[a]->SetUniform("_MVP", MVP::projection() * MVP::modelview());
			r->_materials[a]->Bind();
			r->_mesh->_elos[a]->Bind();
			glDrawElements(GL_TRIANGLES, r->_mesh->_matTriangles[a].size() * 3, GL_UNSIGNED_INT, 0);
			r->_mesh->_elos[a]->Unbind();
			r->_materials[a]->Unbind();
		}
		r->_mesh->_vao->Unbind();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//RenderSky(scene, cam);

	glViewport(0, 0, Display::width(), Display::height());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

CE_END_BK_NAMESPACE