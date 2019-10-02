#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

int UI_Ext::_maxLayer = 0;
int UI_Ext::_currentLayer = 0;
Input::State UI_Ext::_inputState = {};

void UI_Ext::PreLoop() {
    _maxLayer = _currentLayer;
    _currentLayer = 0;
    _inputState = Input::state();
    if (_maxLayer > 0) {
        Input::Clear();
    }
}

void UI_Ext::IncLayer() {
    _currentLayer++;
    if (_currentLayer == _maxLayer) {
        Input::state(_inputState);
    }
    else {
        Input::Clear();
    }
}

int UI_Ext::TextFieldI(const Rect& r, const int& i) {
	return StrExt::ToInt(UI::I::TextField(r, std::to_string(i), Color(0.2f, 0.8f)), i);
}

float UI_Ext::TextFieldF(const Rect& r, const float& f) {
    return StrExt::ToFloat(UI::I::TextField(r, std::to_string(f), Color(0.2f, 0.8f)), f);
}

Vec3 UI_Ext::TextFieldV3(const Rect& r, const Vec3& v) {
    const float w = std::floor(r.w() / 3);
    return Vec3(
        StrExt::ToFloat(UI::I::TextField(Rect(r.x(), r.y(), w - 1, r.h()), std::to_string(v.x), Color(0.3f, 0.1f, 0.1f, 0.8f)), v.x),
        StrExt::ToFloat(UI::I::TextField(Rect(r.x() + w, r.y(), w - 1, r.h()), std::to_string(v.y), Color(0.1f, 0.3f, 0.1f, 0.8f)), v.y),
        StrExt::ToFloat(UI::I::TextField(Rect(r.x() + w*2, r.y(), w, r.h()), std::to_string(v.z), Color(0.1f, 0.1f, 0.3f, 0.8f)), v.z)
    );
}

CE_END_ED_NAMESPACE