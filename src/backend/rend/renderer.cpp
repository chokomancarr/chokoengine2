#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/skyFrag.h"
#include "glsl/probeLightFrag.h"
#include "glsl/transoverlayfrag.h"

/* The renderer backend
 *
 * GBuffers:
 *  Opaque pass:
 *   Buffer 0:
 *     RGB8        //Diffuse Color
 *   Buffer 1:
 *     XYZ16       //Normal vector
 *   Buffer 2:
 *     R8          //Metalness
 *     G8          //Roughness
 *     B8          //Occlusion
 *     A8          //Flags
 * 
 *  Transparent pass:
 *   Buffer 0:
 *     RGB8        //Diffuse Color
 *     A8          //Alpha
 *   Buffer 1:
 *     XYZ16       //Normal vector
 *   Buffer 2:     * metalness is 0
 *     R8          //index of refraction
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
Shader Renderer::probeShad;
Shader Renderer::transOverlayShad;

std::vector<Camera> Renderer::cameras;
std::vector<Light> Renderer::lights;
std::vector<MeshRenderer> Renderer::orends, Renderer::trends;
std::vector<LightProbe> Renderer::probes;

void Renderer::ScanObjects(const std::vector<SceneObject>& oo) {
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
			case ComponentType::LightProbe:
				probes.push_back(static_cast<LightProbe>(c));
				break;
			default:
				break;
			}
		}
		ScanObjects(o->children());
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

void Renderer::RenderScene(const RenderTarget& tar, const RenderTarget& ttar, const Mat4x4& p, const FrameBuffer& gbuf
		, std::function<void()> preBlit, bool useProbes) {
	const auto _w = tar->_width;
	const auto _h = tar->_height;

	const auto ip = p.inverse();

	glViewport(0, 0, _w, _h);

	//opaque pass
	gbuf->Bind();
	gbuf->Clear();

	glBlendFunc(GL_ONE, GL_ZERO);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	for (auto& r : orends) {
		RenderMesh(r, p);
	}

	gbuf->Unbind();

	glBlendFunc(GL_ONE, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);

	//we render to the temporary target
	//because we want to multi-sample it later
	//for the transparent pass
	ttar->BindTarget();

	if (preBlit) preBlit();

	if (useProbes) {
		for (auto& p : probes) {
			ApplyLightProbe(p, _w, _h, gbuf, ip);
		}
	}
	RenderSky(_w, _h, gbuf, ip, false);
	
	for (auto& l : lights) {
		switch(l->_type) {
		case LightType::Point:
			RenderLight_Point(l, gbuf, ip, ttar, false);
			break;
		case LightType::Spot:
			RenderLight_Spot(l, gbuf, ip, ttar, false);
			break;
		case LightType::Directional:
			//RenderLight_Directional(l, cam);
			break;
		}
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuf->_pointer);

	glBlitFramebuffer(0, 0, _w, _h, 0, 0, _w, _h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	ttar->UnbindTarget();

	//transparent pass
	gbuf->Bind();
	gbuf->Clear();

	glBlendFunc(GL_ONE, GL_ZERO);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	for (auto& r : trends) {
		RenderMesh(r, p);
	}

	gbuf->Unbind();

	glBlendFunc(GL_ONE, GL_ONE);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_CULL_FACE);

	tar->BindTarget();

	if (preBlit) preBlit();

	/*if (useProbes) {
		for (auto& p : probes) {
			ApplyLightProbe(p, _w, _h, gbuf, ip);
		}
	}*/
	RenderSky(_w, _h, gbuf, ip, true);
	
	for (auto& l : lights) {
		switch(l->_type) {
		case LightType::Point:
			RenderLight_Point(l, gbuf, ip, tar, true);
			break;
		case LightType::Spot:
			RenderLight_Spot(l, gbuf, ip, tar, true);
			break;
		case LightType::Directional:
			//RenderLight_Directional(l, cam);
			break;
		}
	}

	tar->UnbindTarget();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ZERO);

	tar->BindTarget();
	
	transOverlayShad->Bind();
	glUniformMatrix4fv(transOverlayShad->Loc(0), 1, false, &p[0][0]);
	glUniformMatrix4fv(transOverlayShad->Loc(1), 1, false, &ip[0][0]);
	glUniform2f(transOverlayShad->Loc(2), _w, _h);
	auto cp = ip * Vec4(0, 0, -1, 1);
	cp /= cp.w;
	glUniform3f(transOverlayShad->Loc(3), cp.x, cp.y, cp.z);
	glUniform1i(transOverlayShad->Loc(4), 0);
	glActiveTexture(GL_TEXTURE0);
	tar->Bind();
	glUniform1i(transOverlayShad->Loc(5), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[1]->_pointer);
	glUniform1i(transOverlayShad->Loc(6), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuf->_texs[2]->_pointer);
	glUniform1i(transOverlayShad->Loc(7), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuf->_depth->_pointer);
	glUniform1i(transOverlayShad->Loc(8), 4);
	glActiveTexture(GL_TEXTURE4);
	ttar->Bind();
	glUniform1i(transOverlayShad->Loc(9), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, ttar->_depth);
	_emptyVao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	_emptyVao->Unbind();
	transOverlayShad->Unbind();

	tar->UnbindTarget();
}

void Renderer::RenderCamera(Camera& cam) {
	for (auto& c : cam->_object.lock()->_components) {
		c->OnPreRender();
	}

	const auto v = cam->object()->transform()->worldMatrix().inverse();

	const auto& tar = cam->_target;
	const auto _w = (!tar) ? Display::width() : tar->_width;
	const auto _h = (!tar) ? Display::height() : tar->_height;

	const auto& vp = cam->_lastViewProjectionMatrix = glm::perspectiveFov<float>(cam->fov() * Math::deg2rad, _w, _h, cam->nearClip(), cam->farClip())
		* v;

	auto& gbuf = cam->_deferredBuffer;
	auto& btar = cam->_blitTargets[0];

	if (!gbuf) {
		gbuf = FrameBuffer_New(_w, _h, {
			GL_RGBA, GL_RGB32F, GL_RGBA, GL_RGBA
		});
	}
	if (!btar) {
		for (auto& t : cam->_blitTargets) {
			t = RenderTarget::New(_w, _h, true, true);
		}
	}

	RenderScene(btar, cam->_blitTargets[1], vp, gbuf, [&]() {
		if ((cam->_clearType == CameraClearType::Color)
				|| (cam->_clearType == CameraClearType::ColorAndDepth)) {
			glClearBufferfv(GL_COLOR, 0, &cam->_clearColor[0]);
		}
		if ((cam->_clearType == CameraClearType::Depth)
			|| (cam->_clearType == CameraClearType::ColorAndDepth))
			glClearBufferfv(GL_DEPTH, 0, &cam->_clearDepth);

		for (auto& c : cam->_object.lock()->_components) {
			c->OnPreBlit();
		}
	}, true);

	int sw = 0;
	for (auto& e : cam->_effects) {
		e->Apply(cam->_blitTargets[sw], cam->_blitTargets[1-sw], cam->_blitTargets[2], gbuf);
		sw = 1 - sw;
	}

	if (sw == 1) {
		std::swap(cam->_blitTargets[0], cam->_blitTargets[1]);
	}

	btar->BindTarget();
	glBlendFunc(GL_ONE, GL_ZERO);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	static int n = 0;

	if (Input::KeyDown(InputKey::F)) {
		n = (++n) % 5;
	}

	GI::Voxelizer::DrawDebugAO(vp, n);// std::fmod(Time::time(), 2.0f) > 1 ? 1 : 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	btar->Blit(tar, nullptr);

	tar->BindTarget();
	for (auto& c : cam->_object.lock()->_components) {
		c->OnPostBlit();
	}

	tar->UnbindTarget();

	glViewport(0, 0, Display::width(), Display::height());

	for (auto& c : cam->_object.lock()->_components) {
		c->OnPostRender();
	}
}

void Renderer::RenderSky(int w, int h, const FrameBuffer& gbuf, const Mat4x4& ip, bool tr) {
	if (!Scene::_sky || !Scene::_sky->loaded()) {
		gbuf->Bind(true);
		glReadBuffer(GL_COLOR_ATTACHMENT3);
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		gbuf->Unbind(true);
	}
	else {
		skyShad->Bind();
		glUniformMatrix4fv(skyShad->Loc(0), 1, GL_FALSE, &ip[0][0]);
		glUniform2f(skyShad->Loc(1), w, h);
		glUniform1i(skyShad->Loc(2), false); //is_ortho
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
		glUniform1f(skyShad->Loc(10), tr ? 1 : 0);
		_emptyVao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		_emptyVao->Unbind();
		skyShad->Unbind();
	}
}

bool Renderer::Init() {
	_emptyVao = std::make_shared<_VertexArray>();

	(skyShad = Shader::New(glsl::minVert, glsl::skyFrag))
		->AddUniforms({ "_IP", "screenSize", "isOrtho", "inGBuf0", "inGBuf1", "inGBuf2", "inGBuf3", "inGBufD", "inSky", "skyStrength", "transparent" });
	(probeShad = Shader::New(glsl::minVert, glsl::probeLightFrag))
		->AddUniforms({ "_IP", "screenSize", "inGBuf0", "inGBuf1", "inGBuf2", "inGBuf3", "inGBufD", "cubemap", "skyStrength" });
	(transOverlayShad = Shader::New(glsl::minVert, glsl::transOverlayFrag))
		->AddUniforms({ "_P", "_IP", "screenSize", "camPos", "trTex", "trNrm", "trPrm", "trDep", "opTex", "opDep" });

	return !!skyShad && !!probeShad && InitLightShaders() && GI::Voxelizer::InitShaders();
}

void Renderer::Render() {
	cameras.clear();
	lights.clear();
	orends.clear();
	trends.clear();
	probes.clear();

	ScanObjects(Scene::objects());

	for (auto& p : probes) {
		if (p->_dirty || (p->updateFrequency() == LightProbeUpdateFrequency::Realtime)) {
			RenderLightProbe(p);
			p->_dirty = false;
		}
	}

	GI::Voxelizer::Bake();

	for (auto& c : cameras) {
		RenderCamera(c);
	}
}

CE_END_BK_NAMESPACE