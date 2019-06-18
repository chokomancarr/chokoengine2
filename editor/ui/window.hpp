#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EWindow {
protected:
	virtual void DrawMenu() = 0;
public:
	virtual ~EWindow() = default;

	Rect position;

	virtual bool Init() = 0;
	void Draw();
};

CE_END_ED_NAMESPACE