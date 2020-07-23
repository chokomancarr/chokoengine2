#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
	static VertexArray _emptyVao;

	static Shader skyShad;

	static Shader pointLightShad;
	static Shader spotLightShad;
	static Shader probeShad;
	static Shader transOverlayShad;

public:
	static std::vector<Camera> cameras;
	static std::vector<Light> lights;
	static std::vector<MeshRenderer> orends, trends;
	static std::vector<LightProbe> probes;
	static std::vector<ParticleSystem> parsyss;
	static Background sky;

private:

	static bool InitLightShaders();

	static void ScanObjects(const std::vector<SceneObject>&);

	static void RenderMesh(const MeshRenderer& rend, const Mat4x4& P);

	static void RenderScene(const RenderTarget& tar, const RenderTarget& ttar, const Mat4x4& p, const FrameBuffer& gbuf, std::function<void()> preBlit, bool useProbes);

	static void RenderProbe_CubeMap(const CubeMap&);

	static void RenderSky(int w, int h, const FrameBuffer& gbuf, const Mat4x4& ip, bool tr);

	static void RenderLight_Point(const Light&, const FrameBuffer& gbuf, const Mat4x4& ip, const RenderTarget&, bool tr);
	static void RenderLight_Point_Shadow(const Light&);
	
	static void RenderLight_Spot(const Light&, const FrameBuffer& gbuf, const Mat4x4& ip, const RenderTarget&, bool tr);
	static void RenderLight_Spot_Shadow(const Light&);

	static void RenderLight_Directional(const Light&, const Camera&);

	static void RenderLightProbe(const LightProbe&);

	static void ApplyLightProbe(const LightProbe&, int w, int h, const FrameBuffer& gbuf, const Mat4x4& ip);

public:
	CE_GET_ST_MEMBER(emptyVao);

	static bool Init();

	static void RegisterScene(const Scene& scene);

	static void RenderAuxProbes();

	static void RenderAuxLights();

	static void RenderAuxiliary();

	static void Render(const Scene& scene);

	static void RenderCamera(const Camera& cam);

	friend class GI;

	class Particles;
};

CE_END_BK_NAMESPACE

#include "renderer_particles.hpp"