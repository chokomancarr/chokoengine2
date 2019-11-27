#include "chokoeditor.hpp"
#include "glsl/uiTexVert.h"
#include "glsl/colorpicker_h.h"
#include "glsl/colorpicker_sv.h"

CE_BEGIN_ED_NAMESPACE

std::shared_ptr<EO_ColorPicker> EO_ColorPicker::instance = nullptr;

EO_ColorPicker::EO_ColorPicker() {
	(hShad = Shader::New(glsl::uiTexVert, glsl::colorPickerH))
		->AddUniforms({  });
	(svShad = Shader::New(glsl::uiTexVert, glsl::colorPickerSV))
		->AddUniform("col", ShaderVariableType::Vec3);
	(hMat = Material::New())->shader(hShad);
	(svMat = Material::New())->shader(svShad);
}

void EO_ColorPicker::Draw() {
	UI::Rect(Rect(pos.x, pos.y, 150, 200), Color(0.1f, 0.7f));
	if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up && !Rect(pos.x, pos.y, 150, 200).Contains(Input::mouseDownPosition())) {
		active = false;
	}
	const auto& vl = state->get();
	Color hb = Color(Math::Clamp(std::abs(vl.h * 6 - 3) - 1.f, 0.f, 1.f),
		1 - Math::Clamp(std::abs(vl.h * 6 - 2) - 1.f, 0.f, 1.f),
		1 - Math::Clamp(std::abs(vl.h * 6 - 4) - 1.f, 0.f, 1.f), 1);
	svMat->SetUniform("col", Vec3(hb.r, hb.g, hb.b));
	UI::Rect(Rect(pos.x + 2, pos.y + 2, 120, 120), svMat);
	auto sv = UI::I::Slider2DTr(Rect(pos.x + 2, pos.y + 2, 120, 120), Vec2(0, 1), Vec2(1, 0), Vec2(vl.s, vl.v));

	auto h = UI::I::SliderYTr(Rect(pos.x + 127, pos.y + 2, 18, 120), Vec2(0, 1), vl.h);
	UI::Rect(Rect(pos.x + 127, pos.y + 2, 18, 120), hMat);

	if (sv.x != vl.s || sv.y != vl.v || h != vl.h) {
		state->set(Color::FromHSV(h, sv.x, sv.y));
	}
}

void EO_ColorPicker::Reg(const Rect& pos, Color& tar) {
	auto i = GetInstance<EO_ColorPicker>();
	i->pos = Vec2(pos.x(), pos.y2());
	i->state = std::unique_ptr<StateR>(new StateR(tar));
	i->active = true;
	tar.ComputeHSV();
}

CE_END_ED_NAMESPACE