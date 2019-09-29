#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/skyFrag.h"

/* The renderer backend
 *
 * GBuffers:
 *   Buffer 0:
 *     R8, G8, B8  //Diffuse Color
 *   Buffer 1:
 *     XYZ16       //Normal vector
 *   Buffer 2:
 *     R8          //Metalness
 *     G8          //Roughness
 *     B8          //Occlusion
 *     A8          //Flags
 * 
 * Notes:
 *   Computing Z from XY of normal map produces slight but noticeable artifacts, even in 32 bits
 *     -> Maybe increasing the resolution in small values will fix this? Does it matter for floats?
 *   Binary metalness parameter doesn't look very nice on borders (layered materials)
 */

CE_BEGIN_BK_NAMESPACE

VertexArray Renderer::_emptyVao;
Shader Renderer::skyShad;
Shader Renderer::pointLightShad;
Shader Renderer::spotLightShad;

void Renderer::ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras,
		std::vector<Light>& lights, std::vector<MeshRenderer>& orends, std::vector<MeshRenderer>& trends) {
	for (auto& o : oo) {
		for (auto& c : o->_components) {
			switch(c->componentType) {
			case ComponentType::Camera:
				cameras.push_back(static_cast<Camera>(c));
				break;
			case ComponentType::Light:
				lights.push_back(static_cast<Light>(c));
				break;
			case ComponentType::MeshRenderer: {
				const auto r = static_cast<MeshRenderer>(c);
				bool iso = false, ist = false;
				for (auto& m : r->materials()) {
					if (!m || !m->shader()) continue;
					if (m->shader()->_queue == ShaderQueue::Transparent) {
						ist = true;
					}
					else {
						iso = true;
					}
				}
				if (iso)
					orends.push_back(r);
				if (ist)
					trends.push_back(r);
				break;
			}
			default:
				break;
			}
		}
		ScanObjects(o->children(), cameras, lights, orends, trends);
	}
}

void Renderer::RenderMesh(const MeshRenderer& rend, const Mat4x4& P) {
	const auto& mesh = rend->_mesh;
	if (!mesh) return;
	const auto& MV = rend->object()->transform()->worldMatrix();

	const auto& vao = (rend->_modifiers.size() > 0) ? rend->_modifiers.back()->result : mesh->_vao;

	vao->Bind();
	for (size_t a = 0; a < rend->_mesh->materialCount(); a++) {
		const auto& mat = rend->_materials[a];
		if (!mat) continue;
		mat->shader()->ApplyFlags();
		mat->SetUniform("_MV", MV);
		mat->SetUniform("_P", P);
		mat->SetUniform("_MVP", P * MV);
		mat->Bind();
		mesh->_elos[a]->Bind();
		glDrawElements(GL_TRIANGLES, mesh->_matTriangles[a].size() * 3, GL_UNSIGNED_INT, 0);
		mesh->_elos[a]->Unbind();
		mat->Unbind();
	}
	vao->Unbind();
}

void Renderer::RenderCamera(Camera& cam, const std::vector<Light>& lights, const std::vector<MeshRenderer>& orends, const std::vector<MeshRenderer>& trends) {
	for (auto& c : cam->_object.lock()->_components) {
		c->OnPreRender();
	}

	const auto& tar = cam->_target;
	const auto _w = (!tar) ? Display::width() : tar->_width;
	const auto _h = (!tar) ? Display::height() : tar->_height;

	MVP::Switch(true);
	MVP::Clear();
	MVP::Mul(glm::perspectiveFov<float>(cam->fov() * Math::deg2rad, _w, _h, cam->nearClip(), cam->farClip()));
	MVP::Push();
	MVP::Mul(cam->object()->transform()->worldMatrix().inverse());

	const auto& ip = (cam->_lastViewProjectionMatrix = MVP::projection()).inverse();

	glViewport(0, 0, _w, _h);

	auto& gbuf = cam->_deferredBuffer;
	if (!gbuf) {
		gbuf = FrameBuffer_New(_w, _h, {
			GL_RGBA, GL_RGB32F, GL_RGBA, GL_RGBA
		});
		for (auto& t : cam->_blitTargets) {
			t = RenderTarget::New(_w, _h, true, false);
		}
	}
	gbuf->Bind();
	gbuf->Clear();

	glBlendFunc(GL_ONE, GL_ZERO);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	for (auto& r : orends) {
		RenderMesh(r, cam->_lastViewProjectionMatrix);
	}

	gbuf->Unbind();

	glBlendFunc(GL_ONE, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);

	const auto& btar = cam->_blitTargets[0];
	btar->BindTarget();

	for (auto& c : cam->_object.lock()->_components) {
		c->OnPreBlit();
	}

	if ((cam->_clearType == CameraClearType::Color)
			|| (cam->_clearType == CameraClearType::ColorAndDepth)) {
		glClearBufferfv(GL_COLOR, 0, &cam->_clearColor[0]);
	}
	if ((cam->_clearType == CameraClearType::Depth)
		|| (cam->_clearType == CameraClearType::ColorAndDepth))
		glClearBufferfv(GL_DEPTH, 0, &cam->_clearDepth);

	RenderSky(cam);

	for (auto& l : lights) {
		switch(l->_type) {
		case LightType::Point:
			RenderLight_Point(l, cam);
			break;
		case LightType::Spot:
			RenderLight_Spot(l, cam, ip, btar, orends);
			break;
		case LightType::Directional:
			RenderLight_Directional(l, cam);
			break;
		}
	}

	btar->UnbindTarget();

	int sw = 0;
	for (auto& e : cam->_effects) {
		e->Apply(cam->_blitTargets[sw], cam->_blitTargets[1-sw], cam->_blitTargets[2], gbuf);
		sw = 1 - sw;
	}

	if (sw == 1) {
		std::swap(cam->_blitTargets[0], cam->_blitTargets[1]);
	}

	btar->BindTarget();
	for (auto& c : cam->_object.lock()->_components) {
		c->OnPostBlit();
	}
	btar->UnbindTarget();

	btar->Blit(tar, nullptr);

	glViewport(0, 0, Display::width(), Display::height());
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& c : cam->_object.lock()->_components) {
		c->OnPostRender();
	}
}

void Renderer::RenderSky(const Camera& cam) {
	const auto ip = glm::inverse(MVP::projection());

	const auto& tar = cam->_target;
	const auto& gbuf = cam->_deferredBuffer;
	skyShad->Bind();
	glUniformMatrix4fv(skyShad->Loc(0), 1, GL_FALSE, &ip[0][0]);
	glUniform2f(skyShad->Loc(1), tar->_width, tar->_height);
	glUniform1i(skyShad->Loc(2), cam->_orthographic);
	glUniform1i(skyShad->Loc(3), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[0]->_pointer);
	glUniform1i(skyShad->Loc(4), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[1]->_pointer);
	glUniform1i(skyShad->Loc(5), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[2]->_pointer);
	glUniform1i(skyShad->Loc(6), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[3]->_pointer);
	glUniform1i(skyShad->Loc(7), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gbuf->_depth->_pointer);
	glUniform1i(skyShad->Loc(8), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Scene::_sky->_pointer);
	glUniform1f(skyShad->Loc(9), Scene::_sky->_brightness);
	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	skyShad->Unbind();
}

bool Renderer::Init() {
	_emptyVao = std::make_shared<_VertexArray>();

	(skyShad = Shader::New(glsl::minVert, glsl::skyFrag))
		->AddUniforms({ "_IP", "screenSize", "isOrtho", "inGBuf0", "inGBuf1", "inGBuf2", "inGBuf3", "inGBufD", "inSky", "skyStrength" });

	return !!skyShad && InitLightShaders();
}

void Renderer::Render() {
	std::vector<Camera> cameras;
	std::vector<Light> lights;
	std::vector<MeshRenderer> orends, trends;

	ScanObjects(Scene::objects(), cameras, lights, orends, trends);

	for (auto& c : cameras) {
		RenderCamera(c, lights, orends, trends);
	}
}

CE_END_BK_NAMESPACE