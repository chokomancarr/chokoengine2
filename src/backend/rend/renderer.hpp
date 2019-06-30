#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
	static VertexObject _emptyVao;

	static Shader skyShad;

	static Shader pointLightShad;

    static void ScanObjects(const std::vector<SceneObject>&, std::vector<Camera>&, std::vector<Light>&, std::vector<MeshRenderer>&);

    static void RenderCamera(const Scene& scene, const Camera& cam, const std::vector<Light> lights, const std::vector<MeshRenderer> rends);

	static void RenderSky(const Scene&, const Camera&);

	static void RenderLight_Point(const Scene&, const Light&, const Camera&);
	static void RenderLight_Spot(const Scene&, const Light&, const Camera&);
	static void RenderLight_Directional(const Scene&, const Light&, const Camera&);

public:
	CE_GET_ST_MEMBER(emptyVao);

	static bool Init();

    static void Render(const Scene& scene);
};

CE_END_BK_NAMESPACE