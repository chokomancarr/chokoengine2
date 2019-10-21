#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::shared_ptr<EO_SelectRef> EO_SelectRef::instance = nullptr;

void EO_SelectRef::Draw() {
	UI::Rect(Rect(10, 10, 200, 400), Color(0.1f, 0.7f));
	if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up && !Rect(10, 10, 200, 400).Contains(Input::mouseDownPosition())) {
		active = false;
	}
	float y = 12;
	if (!state->comp) {
		for (auto& s : _assets) {
			if (UI::I::Button(Rect(12, y, 200, 16), UIButtonStyle(Color(0.1f, 0.7f)), s) == InputMouseStatus::HoverUp) {
				state->set((Object)EAssetList::Get(_assetType, s));
				active = false;
			}
			y += 16;
		}
	}
}

CE_END_ED_NAMESPACE