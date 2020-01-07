#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<std::shared_ptr<EWindow>> EWindowManager::windows;

std::vector<EDropdownMenu> EWindowManager::menus;

void EWindowManager::Init() {
	std::vector<EDropdownMenu>* items;
#define addm(nm) menus.push_back(EDropdownMenu(#nm)); items = &menus.back().items
#define addi(nm, ...) {auto op = EDropdownMenu(#nm);\
		op.callback = ECallbackCaller(__VA_ARGS__);\
		items->push_back(op);}

	addm(File);
	addi(Open Project);
	addi(Close Project);
	addi(New Scene);
	addi(Save Scene, CallbackSig::GLOBAL_SAVE);
	addi(Save Scene As, CallbackSig::GLOBAL_SAVE_AS);

	addm(Object);
	//addi(New Object, CallbackSig::OBJECT_ADD_EMPTY);

#undef addi
#undef addm

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
	windows.back()->Init();
	windows.push_back(std::make_shared<EW_Inspector>());
	SETPOS(Rect(750, 0, 250, 600));
	windows.back()->Init();
}

void EWindowManager::Update() {
	for (auto& w : windows) {
		w->position = (w->_position * Vec2(Display::width(), Display::height() - 21)).sub(0, 21, 1, -20);
		w->Update();
	}
}

void EWindowManager::Draw() {
	static const auto& logotex = EIcons::icons["logo"];
	UI::Rect(Rect(0, 0, Display::width(), 20), Color(0, 0.7f));
	UI::Texture(Rect(2, 2, 16, 16), logotex, Color(0.7f));
	EO_Dropdown::DrawMenus(menus, Vec2(20, 2));

	if (UI::I::Button(Rect(100, 2, 50, 16), Color(0.3f, 0.6f, 0.3f), "play") == InputMouseStatus::HoverUp) {
		ECallbackManager::Invoke(CallbackSig::GLOBAL_PLAY);
	}
	if (UI::I::Button(Rect(160, 2, 50, 16), Color(0.6f, 0.3f, 0.3f), "stop") == InputMouseStatus::HoverUp) {
		ECallbackManager::Invoke(CallbackSig::GLOBAL_STOP);
	}
	if (UI::I::Button(Rect(250, 2, 50, 16), Color(0.3f, 0.3f, 0.3f), "build_dbg") == InputMouseStatus::HoverUp) {
		ECallbackManager::Invoke(CallbackSig::GLOBAL_BUILD_DEBUG);
	}
	if (UI::I::Button(Rect(310, 2, 50, 16), Color(0.3f, 0.3f, 0.3f), "build_rel") == InputMouseStatus::HoverUp) {
		ECallbackManager::Invoke(CallbackSig::GLOBAL_BUILD);
	}

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
