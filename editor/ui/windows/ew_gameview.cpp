#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_GameView::DrawMenu() {
	const Rect& r = Rect(position.x(), position.y() + 20, position.w(), position.h() - 20);
	glBlendFunc(GL_ONE, GL_ZERO);
	UI::Texture(r, _target);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

EW_GameView::EW_GameView() : EWindow("Game") {

}

bool EW_GameView::Init() {
	_target = RenderTarget::New((uint)position.w(), (uint)(position.h() - 20), true, true);
    return true;
}

void EW_GameView::Update() {
	auto sw = (uint)position.w(), sh = (uint)(position.h() - 20);

	if (sw != _target->width() || sh != _target->height()) {
		_target = RenderTarget::New(sw, sh, true, true);
	}
}

void EW_GameView::Render() {
    auto cm = ChokoEditor::scene->GetActiveCameras();
    for (auto& c : cm) {
        if (!c->target()) {
            c->target(_target);
            ChokoEditor::scene->RenderCameras({c});
            c->target(nullptr);
        }
    }
}

CE_END_ED_NAMESPACE