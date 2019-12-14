#pragma once
#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

class SceneManager {
    static std::vector<std::string> scenePaths;

    const Scene& activeScene();

    void Init();

    void Load(int);
};

CE_END_PL_NAMESPACE

#include "comp/script_loader_base.hpp"