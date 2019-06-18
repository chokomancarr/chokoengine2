#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
	static VertexObject emptyVao;

	static Shader skyShad;

    static void ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras);

	static void RenderSky(const Scene& scene, const Camera& cam);

public:
	static bool Init();

    static void Render(const Scene& scene);
    static void RenderCamera(const Scene& scene, const Camera& cam);
};

CE_END_BK_NAMESPACE