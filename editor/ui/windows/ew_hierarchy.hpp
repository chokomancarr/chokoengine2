#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_Hierarchy : public EWindow {
    std::unordered_map<int, bool> _expanded;

    void DrawMenuObject(int& off, const std::vector<SceneObject>& oo, int level);
protected:
	void DrawMenu() override;
	
public:
	bool Init() override;
};

CE_END_ED_NAMESPACE