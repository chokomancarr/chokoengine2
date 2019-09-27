#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
	static VertexArray _emptyVao;

	static Shader skyShad;

	static Shader pointLightShad;
	static Shader spotLightShad;

	static bool InitLightShaders();

    static void ScanObjects(const std::vector<SceneObject>&, std::vector<Camera>&, std::vector<Light>&, std::vector<MeshRenderer>&, std::vector<MeshRenderer>&);

	static void RenderMesh(const MeshRenderer& rend);

    static void RenderCamera(Camera& cam, const std::vector<Light>& lights, const std::vector<MeshRenderer>& orends, const std::vector<MeshRenderer>& trends);

	static void RenderSky(const Camera&);

	static void RenderLight_Point(const Light&, const Camera&);
	static void RenderLight_Spot(const Light&, const Camera&);
	static void RenderLight_Directional(const Light&, const Camera&);

	static void RenderLightProbe(const LightProbe&);

public:
	CE_GET_ST_MEMBER(emptyVao);

	static bool Init();

    static void Render();
};

CE_END_BK_NAMESPACE