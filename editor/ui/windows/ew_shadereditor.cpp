#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

VShader EW_ShaderEditor::target = nullptr;

void EW_ShaderEditor::DrawMenu() {
	if (!target) return;
	float x0 = position.x() + 2;
	float y0 = position.y() + 20;
	for (auto& n : target->nodes) {
		float x = n->pos.x + x0;
		float y = n->pos.y + y0;
		UI::Rect(Rect(x, y, 150, 100), Color(0.1f, 0.7f));
		UI::Label(Rect(x + 2, y, 150, 16), n->name, Color(1));
		y += 20;
		for (auto& i : n->inputs) {
			UI::Label(Rect(x + 5, y, 100, 18), i.name, Color(1));
			if (i.target.i > -1) {
				const auto& n2 = i.target.nd.lock();
				UI::Line(Vec2(x0 + n2->pos.x + 150, y0 + n2->pos.y + 20 + n2->inputs.size() * 18 + 13 + i.target.i * 18), Vec2(x, y + 9), Color::red());
			}
			y += 18;
		}
		y += 4;
		UI::defaultFont()->alignment(FontAlign::TopRight);
		for (auto& i : n->outputs) {
			UI::Label(Rect(x, y, 145, 18), i.name, Color(1));
			y += 18;
		}
		UI::defaultFont()->alignment(FontAlign::TopLeft);
	}
}

EW_ShaderEditor::EW_ShaderEditor() : EWindow("Shader Editor") {}

bool EW_ShaderEditor::Init() {
	return true;
}

CE_END_ED_NAMESPACE