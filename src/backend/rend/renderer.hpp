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

    static void RenderCamera(Camera& cam);

	static void RenderProbe_CubeMap(const CubeMap&);

	static void RenderSky(const Camera&);

	static void RenderLight_Point(const Light&, const Camera&, const Mat4x4& ip, const RenderTarget&);
	static void RenderLight_Point_Shadow(const Light&);
	
	static void RenderLight_Spot(const Light&, const Camera&, const Mat4x4& ip, const RenderTarget&);
	static Mat4x4 RenderLight_Spot_Shadow(const Light&);

	static void RenderLight_Directional(const Light&, const Camera&);

	static void RenderLightProbe(const LightProbe&);

public:
	CE_GET_ST_MEMBER(emptyVao);

	static bool Init();

    static void Render();
};

CE_END_BK_NAMESPACE