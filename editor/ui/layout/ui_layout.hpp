#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EUILayout {
public:
	struct ScrollState {
		float max;
		float off;
		Rect rng;
	};
	static float BeginScroll(const Rect&, ScrollState&);
	static void EndScroll(ScrollState&, float);
};

CE_END_ED_NAMESPACE