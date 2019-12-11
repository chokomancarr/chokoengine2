#pragma once
#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

namespace SceneManager {
    const Scene& activeScene();

    void Init();

    void Load(int);
};

CE_END_PL_NAMESPACE

#include "comp/script_loader_base.hpp"