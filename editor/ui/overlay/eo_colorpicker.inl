#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename F>
void EO_ColorPicker::Reg(const Rect& pos, const Color& val, std::function<void(F)> setter) {
	auto i = GetInstance<EO_ColorPicker>();
	i->pos = Vec2(pos.x(), pos.y2());
	i->state = std::unique_ptr<State<F>>(new State<F>(val, setter));
	i->active = true;
};

CE_END_ED_NAMESPACE