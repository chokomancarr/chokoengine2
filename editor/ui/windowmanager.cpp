#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<std::shared_ptr<EWindow>> EWindowManager::windows;

void EWindowManager::Init() {

}

void EWindowManager::LoadWindows() {
	windows.push_back(std::make_shared<EW_Hierarchy>());
	windows.back()->position = Rect(10, 10, 150, 400);
}

void EWindowManager::Draw() {
	for (auto& w : windows) {
		w->Draw();
	}
}

CE_END_ED_NAMESPACE
