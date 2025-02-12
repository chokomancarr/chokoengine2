#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EWindow {
protected:
	EWindow(const std::string& title);

	virtual void DrawMenu() = 0;
public:

	virtual ~EWindow() = default;

	std::string title;
	Rect _position, position;

	std::vector<EDropdownMenu> menus;

	virtual bool Init() = 0;
	virtual void Refresh() {}
	virtual void Update() {}
	virtual void ActiveUpdate() {}
	void Draw();
	virtual void Render() {}
};

CE_END_ED_NAMESPACE