#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<std::shared_ptr<EWindow>> EWindowManager::windows;

void EWindowManager::Init() {
	EW_SceneView::_Init();
}

void EWindowManager::LoadWindows() {
	const auto ds = Vec2(1.0f / Display::width(), 1.0f / Display::height());
#define SETPOS(vl) windows.back()->position = vl; windows.back()->_position = vl * ds;

	windows.push_back(std::make_shared<EW_SceneView>());
	SETPOS(Rect(0, 0, 600, 350));
	windows.back()->Init();
	windows.push_back(std::make_shared<EW_Browser>());
	SETPOS(Rect(0, 350, 400, 250));
	windows.back()->Init();
	windows.push_back(std::make_shared<EW_GameView>());
	SETPOS(Rect(400, 350, 350, 250));
	windows.back()->Init();
	windows.push_back(std::make_shared<EW_Hierarchy>());
	SETPOS(Rect(600, 0, 150, 350));
	windows.push_back(std::make_shared<EW_Inspector>());
	SETPOS(Rect(750, 0, 250, 600));
}

void EWindowManager::Update() {
	for (auto& w : windows) {
		w->position = w->_position * Vec2(Display::width(), Display::height()) + Rect(0, 0, -1, -1);
		w->Update();
	}
}

void EWindowManager::Draw() {
	for (auto& w : windows) {
		w->Draw();
	}
}

void EWindowManager::Render() {
	for (auto& w : windows) {
		w->Render();
	}
}

CE_END_ED_NAMESPACE
