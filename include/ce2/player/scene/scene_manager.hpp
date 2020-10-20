#pragma once
#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

class SceneManager {
public:
	struct Entry {
		std::string name;
		std::string path;
	};
    static CE_EXPORT std::vector<Entry> scenes;

	static const Scene& activeScene();

	static void Init();

	static Scene Load(int);
	static Scene Load(const std::string&);
};

CE_END_PL_NAMESPACE

#include "comp/script_loader_base.hpp"