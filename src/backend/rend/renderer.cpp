#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

void Renderer::ScanObjects(const std::vector<SceneObject>& oo, std::vector<Camera>& cameras) {
    for (auto& o : oo) {
        auto c = o->GetComponent<Camera>();
        if (!!c) {
            cameras.push_back(c);
        }
        ScanObjects(o->children(), cameras);
    }
}

void Renderer::Render(const Scene& scene) {
    static std::vector<Camera> cameras;

    ScanObjects(scene->objects(), cameras);

    for (auto& c : cameras) {
        RenderCamera(c);
    }
}

void Renderer::RenderCamera(const Camera& cam) {
    MVP::Clear();
    
}

CE_END_BK_NAMESPACE