#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class UI_Ext {

public:
	static int TextFieldI(const Rect& r, const int& s);

	static float TextFieldF(const Rect& r, const float& s);

	static Vec3 TextFieldV3(const Rect& r, const Vec3& v);

	class Layout;
};

CE_END_ED_NAMESPACE

#include "ui_ext_layout.hpp"