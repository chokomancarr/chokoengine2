#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
	static VertexArray _emptyVao;

	static Shader skyShad;

	static Shader pointLightShad;
	static Shader spotLightShad;

	static std::vector<Camera> cameras;
	static std::vector<Light> lights;
	static std::vector<MeshRenderer> orends, trends;

	static bool InitLightShaders();

    static void ScanObjects(const std::vector<SceneObject>&);

	static void RenderMesh(const MeshRenderer& rend, const Mat4x4& P);

	static void RenderScene(const RenderTarget& tar, const Mat4x4& p, FrameBuffer& gbuf, std::function<void()> preBlit);

    static void RenderCamera(Camera& cam);

	static void RenderProbe_CubeMap(const CubeMap&);

	static void RenderSky(int w, int h, const FrameBuffer& gbuf, const Mat4x4& ip);

	static void RenderLight_Point(const Light&, const FrameBuffer& gbuf, const Mat4x4& ip, const RenderTarget&);
	static void RenderLight_Point_Shadow(const Light&);
	
	static void RenderLight_Spot(const Light&, const FrameBuffer& gbuf, const Mat4x4& ip, const RenderTarget&);
	static Mat4x4 RenderLight_Spot_Shadow(const Light&);

	static void RenderLight_Directional(const Light&, const Camera&);

	static void RenderLightProbe(const LightProbe&);

public:
	CE_GET_ST_MEMBER(emptyVao);

	static bool Init();

    static void Render();
};

CE_END_BK_NAMESPACE