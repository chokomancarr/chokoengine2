#pragma once
#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

class SceneManager {
public:
    static CE_EXPORT std::vector<std::string> scenePaths;

	static const Scene& activeScene();

	static void Init();

    static void Load(int);
};

CE_END_PL_NAMESPACE

#include "comp/script_loader_base.hpp"