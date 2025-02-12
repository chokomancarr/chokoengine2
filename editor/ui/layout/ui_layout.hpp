#pragma once
#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

class EUILayout {
public:
	struct ScrollState {
		float max;
		float off;
		Rect rng;
	};
	struct PanState {
		Vec2 max;
		Vec2 off;
		Rect rng;
	};

	static float BeginScroll(const Rect&, ScrollState&);
	static void EndScroll(ScrollState&, float);

	static void BeginScroll(UI_Ext::Layout::InfoSt&, ScrollState&, float maxy);
	static void EndScroll(UI_Ext::Layout::InfoSt&, ScrollState&);

	static Vec2 BeginPan(const Rect& frame, const Rect& content, PanState&);
	static void EndPan(PanState&);
};

CE_END_ED_NAMESPACE