#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_I_SceneObject {
	static std::vector<float> _coffs;

public:
	static void Draw(const SceneObject& o, Rect r);
};

CE_END_ED_NAMESPACE

#include "component.hpp"