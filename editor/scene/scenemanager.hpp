#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESceneManager {

public:
	static void Init();

	static void Load(const std::string& path);

	static void Unload();
};

CE_END_ED_NAMESPACE