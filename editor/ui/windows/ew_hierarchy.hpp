#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_Hierarchy : public EWindow {
    float DrawMenuObject(float& off, const std::vector<SceneObject>& oo, int level);
    void DoExpandCollapse(bool, const std::vector<SceneObject>&);

protected:
	void DrawMenu() override;
	
public:
	bool Init() override;

    void ExpandAll();
    void CollapseAll();
};

CE_END_ED_NAMESPACE