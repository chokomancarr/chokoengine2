#pragma once
#include "chokoeditor.hpp"
#include "ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T>
bool UI_Ext::Layout::HandleButtons(CE_UI_BUTTON_MASK m, std::vector<T>& vec, const T& v) {
	if (!m) return false;
	
	return HandleButtons(m, vec, std::find(vec.begin(), vec.end(), v));
}

template <typename T>
bool UI_Ext::Layout::HandleButtons(CE_UI_BUTTON_MASK m, std::vector<T>& vec, typename std::vector<T>::const_iterator it) {
	if (!m) return false;

	if (m == CE_UI_BUTTON_CLOSE) {
		vec.erase(it);
		return true;
	}
	else if (m == CE_UI_BUTTON_UP) {

	}
	else if (m == CE_UI_BUTTON_DOWN) {

	}

	return false;
}

CE_END_ED_NAMESPACE