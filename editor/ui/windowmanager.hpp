#pragma once
#include "chokoeditor.hpp"

#include "window.hpp"
#include "windows/ew_browser.hpp"

CE_BEGIN_ED_NAMESPACE

class EWindowManager {
public:
	static std::vector<std::unique_ptr<EWindow>> windows;

	static void Init();
	static void LoadWindows();
	static void Draw();
};

CE_END_ED_NAMESPACE