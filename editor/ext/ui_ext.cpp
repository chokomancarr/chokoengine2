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

Vec4 UI_Ext::TextFieldV4(const Rect& r, const Vec4& v) {
	const float w = std::floor(r.w() / 2);
	return Vec4(
		StrExt::ToFloat(UI::I::TextField(Rect(r.x(), r.y(), w - 1, r.h()), std::to_string(v.x), Color(0.3f, 0.1f, 0.1f, 0.8f)), v.x),
		StrExt::ToFloat(UI::I::TextField(Rect(r.x() + w, r.y(), w - 1, r.h()), std::to_string(v.y), Color(0.1f, 0.3f, 0.1f, 0.8f)), v.y),
		StrExt::ToFloat(UI::I::TextField(Rect(r.x(), r.y() + r.h() + 1, w - 1, r.h()), std::to_string(v.z), Color(0.1f, 0.1f, 0.3f, 0.8f)), v.z),
		StrExt::ToFloat(UI::I::TextField(Rect(r.x() + w, r.y() + r.h() + 1, w - 1, r.h()), std::to_string(v.w), Color(0.3f, 0.3f, 0.1f, 0.8f)), v.w)
	);
}

bool UI_Ext::StartDrag(const Rect& r) {
    if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Hold) {
        if (UI::stencilRect().Contains(Input::mouseDownPosition()) && r.Contains(Input::mouseDownPosition())) {
            if (glm::length2(Input::mousePosition() - Input::mouseDownPosition()) > 4) {
                return true;
            }
        }
    }
    return false;
}

CE_END_ED_NAMESPACE