#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_GameView : public EWindow {
	RenderTarget _target;

protected:
	void DrawMenu() override;

public:
	EW_GameView();

	Int2 resolution;

	bool Init() override;

	void Update() override;

	void Render() override;
};

CE_END_ED_NAMESPACE