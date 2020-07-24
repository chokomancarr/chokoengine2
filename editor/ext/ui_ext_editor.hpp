#pragma once
#include "chokoeditor.hpp"
#include "ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

class UI_Ext::Editor {

public:
	
	static void Gradient(Layout::InfoSt&, CE_NS Gradient&);

	static void FCurve(Layout::InfoSt&, CE_NS FCurve&);
};

CE_END_ED_NAMESPACE