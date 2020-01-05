#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_Dropdown : public EOverlay {
    Vec2 pos;
    EDropdownMenu menu;
	bool title;

	void DoDraw(bool title);

public:
	static std::shared_ptr<EO_Dropdown> instance;

    void Draw() override;
    static void Reg(const Vec2& pos, const EDropdownMenu& menu, bool title);

	static void DrawMenus(const std::vector<EDropdownMenu>&, Vec2 pos, bool right = false);
};

CE_END_ED_NAMESPACE