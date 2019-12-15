#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<std::shared_ptr<EWindow>> EWindowManager::windows;

void EWindowManager::Init() {
	EW_SceneView::_Init();
}

void EWindowManager::LoadWindows() {
	windows.push_back(std::make_shared<EW_SceneView>());
	windows.back()->position = Rect(0, 0, 600, 600);
	windows.back()->Init();
	windows.push_back(std::make_shared<EW_Hierarchy>());
	windows.back()->position = Rect(601, 0, 149, 350);
	windows.push_back(std::make_shared<EW_Inspector>());
	windows.back()->position = Rect(751, 0, 249, 600);
}

void EWindowManager::Update() {
	for (auto& w : windows) {
		w->Update();
	}
}

void EWindowManager::Draw() {
	for (auto& w : windows) {
		w->Draw();
	}
}

CE_END_ED_NAMESPACE
