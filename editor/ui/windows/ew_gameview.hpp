#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_GameView : public EWindow {
	RenderTarget _target;

protected:

public:
	EW_GameView();

	void Update() override;

	void Render() override;
};

CE_END_ED_NAMESPACE