#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

VShader EW_ShaderEditor::target = nullptr;

void EW_ShaderEditor::DrawMenu() {
	if (!target) return;
	float y0 = position.y() + 20;
	for (auto& n : target->nodes) {
		float x = n->pos.x;
		float y = n->pos.y + y0;
		UI::Rect(Rect(x, y, 150, 100), Color(0.1f, 0.7f));
		UI::Label(Rect(x + 2, y, 150, 16), n->name, Color(1));
		y += 20;
		for (auto& i : n->inputs) {
			UI::Label(Rect(x, y, 100, 18), i.name, Color(1));
			y += 18;
		}
		y += 4;
		for (auto& i : n->outputs) {
			UI::Label(Rect(x + 20, y, 100, 18), i.name, Color(1));
			y += 18;
		}
	}
}

EW_ShaderEditor::EW_ShaderEditor() : EWindow("Shader Editor") {}

bool EW_ShaderEditor::Init() {
	return true;
}

CE_END_ED_NAMESPACE