#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::shared_ptr<EO_ColorPicker> EO_ColorPicker::instance = nullptr;

void EO_ColorPicker::Draw() {
	UI::Rect(Rect(pos.x, pos.y, 150, 200), Color(0.1f, 0.7f));
	UI::Rect(Rect(pos.x + 2, pos.y + 2, 120, 120), state->get());
}

void EO_ColorPicker::Reg(const Rect& pos, Color& tar) {
	auto i = GetInstance<EO_ColorPicker>();
	i->pos = Vec2(pos.x(), pos.y2());
	i->state = std::unique_ptr<StateR>(new StateR(tar));
	i->active = true;
}

CE_END_ED_NAMESPACE