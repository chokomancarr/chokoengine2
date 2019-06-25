#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
	static VertexObject _emptyVao;

	static Shader skyShad;

    static void ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras, std::vector<MeshRenderer>& rends);

	static void RenderSky(const Scene& scene, const Camera& cam);

public:
	CE_GET_ST_MEMBER(emptyVao);

	static bool Init();

    static void Render(const Scene& scene);
    static void RenderCamera(const Scene& scene, const Camera& cam, const std::vector<MeshRenderer> rends);
};

CE_END_BK_NAMESPACE