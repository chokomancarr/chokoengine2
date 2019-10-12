#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_Dropdown : public EOverlay {
    Vec2 pos;
    EDropdownMenu menu;
	bool title;

	void DoDraw(bool title);

public:
    void Draw() override;
    static void Reg(const Vec2& pos, const EDropdownMenu& menu, bool title);
};

CE_END_ED_NAMESPACE