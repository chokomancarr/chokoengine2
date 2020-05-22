#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::shared_ptr<EO_SelectEnum> EO_SelectEnum::instance = nullptr;

void EO_SelectEnum::Draw() {
	static UIButtonStyle style(
		Color(0, 0), Color(0.3f, 0.5f), Color(0, 0.5f),
		Color(1), Color(1), Color(1)
	);
	const float w = 150;
	const auto isz = entries.size();
	const auto r = Rect(pos.x, pos.y, w, 2 + 17 * isz);
	UI::Rect(r, Color(0.1f, 0.7f));
	float y = pos.y + 2;
	for (auto& e : entries) {
		if (UI::I::Button(Rect(pos.x + 2, y, w - 4, 17), style, e.first) == InputMouseStatus::HoverUp) {
			state->set(e.second);
			active = false;
		}
		y += 17;
	}
	if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up && !r.Contains(Input::mouseDownPosition())) {
		active = false;
	}
}

CE_END_ED_NAMESPACE