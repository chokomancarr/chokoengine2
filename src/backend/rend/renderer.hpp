#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer {
    void ScanObjects(const SceneObject& o, std::vector<Camera>& cameras);

public:
    void Render(const Scene& scene);
};

CE_END_BK_NAMESPACE