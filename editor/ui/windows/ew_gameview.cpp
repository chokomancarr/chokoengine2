#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_GameView::DrawMenu() {
	const Rect& r = Rect(position.x(), position.y() + 20, position.w(), position.h() - 20);
	glBlendFunc(GL_ONE, GL_ZERO);
    if (EPlayer::playing) {
        UI::Texture(position.sub(0, 20, 0, 0), EPlayer::outputImage);
    }
    else {
        UI::Texture(r, _target);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

EW_GameView::EW_GameView() : EWindow("Game") {

}

bool EW_GameView::Init() {
	_target = RenderTarget::New((uint)position.w(), (uint)(position.h() - 20), true, true);
    return true;
}

void EW_GameView::Update() {
    resolution.x = position.w();
    resolution.y = (position.h() - 20);

	if (resolution.x != _target->width() ||
            resolution.y != _target->height()) {
		_target = RenderTarget::New(resolution.x, resolution.y, true, true);
	}

	EPlayer::targetReso = resolution;
}

void EW_GameView::Render() {
    if (EPlayer::playing) {
        EPlayer::targetReso = resolution;
        EPlayer::Sync();
    }
    else {
        auto cm = ChokoEditor::scene->GetActiveCameras();
        for (auto& c : cm) {
            if (!c->target()) {
                c->target(_target);
                ChokoEditor::scene->RenderCameras({c});
                c->target(nullptr);
            }
        }
    }
}

CE_END_ED_NAMESPACE