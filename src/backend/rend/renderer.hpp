#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
    static void ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras);

public:
    static void Render(const Scene& scene);
    static void RenderCamera(const Camera& cam);
};

CE_END_BK_NAMESPACE