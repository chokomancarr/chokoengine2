#pragma once
#include "chokoeditor.hpp"
#include "ce2/parsers/json.hpp"

CE_BEGIN_ED_NAMESPACE

const std::string EXT_PREFAB = ".prefab";

class Prefab {
    static void LoadObject(const JsonObject& data, SceneObject& parent);
    static void LoadComponent(const JsonObject& data, SceneObject& obj);

    static void LoadMeshRenderer(const JsonObject& data, MeshRenderer c);

public:
    static SceneObject Load(const std::string& path);
};

CE_END_ED_NAMESPACE