#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::shared_ptr<EO_SelectRef> EO_SelectRef::instance = nullptr;

void EO_SelectRef::Draw() {
	UI::Rect(Rect(10, 10, 300, 400), Color(0.1f, 0.7f));
	if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up && !Rect(10, 10, 300, 400).Contains(Input::mouseDownPosition())) {
		active = false;
	}
	static EUILayout::ScrollState st = {};
	const auto rect = Rect(10, 10, 300, 400);
	float y = EUILayout::BeginScroll(rect, st);
	if (UI::I::Button(Rect(12, y, 200, 16), UIButtonStyle(Color(0.1f, 0.7f)), "Cancel") == InputMouseStatus::HoverUp) {
		active = false;
	}
	y += 16;
	if (UI::I::Button(Rect(12, y, 200, 16), UIButtonStyle(Color(0.1f, 0.7f)), "None") == InputMouseStatus::HoverUp) {
		state->set(nullptr);
		active = false;
	}
	y += 16;
	switch (state->type) {
	case _State::Type::Asset:
		for (auto& s : _assets) {
			if (UI::I::Button(Rect(12, y, 200, 16), UIButtonStyle(Color(0.1f, 0.7f)), s) == InputMouseStatus::HoverUp) {
				state->set(EAssetList::Get(_assetType, s));
				active = false;
			}
			y += 16;
		}
		break;
	case _State::Type::Comp:

		break;
	case _State::Type::Scr:
		for (auto& s : _assets) {
			if (UI::I::Button(Rect(12, y, 200, 16), UIButtonStyle(Color(0.1f, 0.7f)), s) == InputMouseStatus::HoverUp) {
				state->set(EAssetList::GetScr(s));
				active = false;
			}
			y += 16;
		}
		break;
	}

	EUILayout::EndScroll(st, y);
}

namespace {
	typedef std::function<void(ScriptInfo)> ScrSetFn;
}

void EO_SelectRef::RegScr(const ScriptInfo& info, ScrSetFn setter) {
	auto i = GetInstance<EO_SelectRef>();
	i->state = std::unique_ptr<State<ScriptInfo, ScriptInfo>>(
		new State<ScriptInfo, ScriptInfo>(info, setter));
	i->state->type = _State::Type::Scr;
	i->_assets = EAssetList::GetScrList();
	i->active = true;
}

CE_END_ED_NAMESPACE