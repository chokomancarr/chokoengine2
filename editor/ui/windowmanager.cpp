#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	int mouseoverId = -1;
	int switchingId = -1;
	int maximizeId = -1;
	int focusId = -1;
}

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
	addi(New Scene, CallbackSig::GLOBAL_NEW);
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
	//windows.push_back(std::make_shared<EW_AnimGrapher>());
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
	{
		mouseoverId = -1;
		int i = 0;
		for (auto& w : windows) {
			if (w->position.Contains(Input::mousePosition())) {
				mouseoverId = i;
				break;
			}
			i++;
		}
	}
	if (maximizeId > -1) {
		windows[maximizeId]->position = Display::fullscreenRect().sub(0, 20, 0, 0);
		mouseoverId = maximizeId;
	}
	else {
		for (auto& w : windows) {
			w->position = (w->_position * Vec2(Display::width(), Display::height() - 21)).sub(0, 21, 1, -20);
		}
	}

	if (mouseoverId > -1) {
		if (Input::KeyHold(InputKey::LeftShift) && Input::KeyDown(InputKey::Space)) {
			maximizeId = (maximizeId > -1) ? -1 : mouseoverId;
		}
		if (Input::KeyDown(InputKey::Tab)) {
			switchingId = mouseoverId;
		}
	}
	if (Input::KeyDown(InputKey::Escape)) {
		if (maximizeId > -1) maximizeId = -1;
	}

	if (maximizeId > -1) {
		windows[maximizeId]->Update();
		windows[maximizeId]->ActiveUpdate();
	}
	else {
		for (auto& w : windows) {
			w->Update();
		}
		if (mouseoverId > -1) {
			windows[mouseoverId]->ActiveUpdate();
		}
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
		ECallbackManager::Invoke(CallbackSig::GLOBAL_BUILD_RELEASE);
	}

	if (maximizeId > -1) {
		windows[maximizeId]->Draw();
	}
	else {
		if (focusId > -1) {
			const auto r = windows[focusId]->position;
			UI::BeginStencil(r, true);
			UI::Rect(Display::fullscreenRect(), Color(0, 0.5f));
			UI::EndStencil();
		}
		for (auto& w : windows) {
			w->Draw();
		}
	}

	if (switchingId > -1) {
		UI_Ext::IncLayer();
		const auto r = windows[switchingId]->position;
		UI::Rect(r, Color(0, 0.5f));
		const float px = r.x() + 10;
		float py = r.y() + 10;
		const float pw = std::min(150.f, r.w() - 20);
#define SELWND(nm, comp) \
			if (UI::I::Button(Rect(px, py, pw, 16), Color(0.2, 0.8f), nm) == InputMouseStatus::HoverUp) {\
				const auto pos = windows[switchingId]->_position;\
				auto& wnd = (windows[switchingId] = std::make_shared<comp>());\
				wnd->Init();\
				wnd->_position = pos;\
				switchingId = -1;\
			}\
			py += 20;
		SELWND("3D View", EW_SceneView);
		SELWND("Asset Browser", EW_Browser);
		SELWND("Game Output", EW_GameView);
		SELWND("Hierarchy", EW_Hierarchy);
		SELWND("Inspector", EW_Inspector);
		SELWND("Anim Graph Editor", EW_AnimGrapher);
		//SELWND("Animation", EW_AnimEditor);

		if (Input::KeyDown(InputKey::Escape)) {
			if (switchingId > -1) switchingId = -1;
		}
	}
}

void EWindowManager::Render() {
	for (auto& w : windows) {
		w->Render();
	}
}

void EWindowManager::Focus(EWindow* const w, bool f) {
	if (f) {
		focusId = std::find_if(windows.begin(), windows.end(), [w](const std::shared_ptr<EWindow> w2) {
			return &(*w2) == w;
		}) - windows.begin();
	}
	else focusId = -1;
}

bool EWindowManager::inFocus(EWindow* const w) {
	if (focusId == -1) return false;
	else return &(*windows[focusId]) == w;
}

CE_END_ED_NAMESPACE
