#pragma once
#include "chokoeditor.hpp"

#include "window.hpp"
#include "windows/ew_animgrapher.hpp"
#include "windows/ew_browser.hpp"
#include "windows/ew_gameview.hpp"
#include "windows/ew_hierarchy.hpp"
#include "windows/ew_inspector.hpp"
#include "windows/ew_sceneview.hpp"
#include "windows/ew_shadereditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EWindowManager {
public:
	static std::vector<std::shared_ptr<EWindow>> windows;

	static std::vector<EDropdownMenu> menus;

	static void Init();
	static void LoadWindows();
	static void Update();
	static void Draw();
	static void Render();

	static void Focus(EWindow* const, bool);
	static bool inFocus(EWindow* const);
};

CE_END_ED_NAMESPACE