#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_I_SceneObject {
public:
	static void Draw(const SceneObject& o, UI_Ext::Layout::InfoSt& r);
};

CE_END_ED_NAMESPACE

#include "component.hpp"