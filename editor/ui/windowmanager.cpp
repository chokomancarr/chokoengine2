#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<std::shared_ptr<EWindow>> EWindowManager::windows;

void EWindowManager::Init() {
	EW_SceneView::_Init();
}

void EWindowManager::LoadWindows() {
	windows.push_back(std::make_shared<EW_SceneView>());
	windows.back()->position = Rect(0, 0, 800, 800);
	windows.back()->Init();
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
